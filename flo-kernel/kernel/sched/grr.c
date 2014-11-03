/*
 * Round-Robin Scheduling Class (mapped to the SCHED_GRR policy)
 */

#include "sched.h"

#include <linux/slab.h>

#define BASE_WRR_TIME_SLICE (100 * HZ / 1000) /* time slice of 100ms */

struct load {
	unsigned long nr_running;
	struct rq *rq;
	int cpu;
};

static int can_move_grr_task(struct task_struct *p,
			     struct rq *source,
			     struct rq *target)
{
	/* see __migrate_task() in core.c for details */
	if (!cpumask_test_cpu(target->cpu, tsk_cpus_allowed(p)))
		return 0;
	if (!cpu_online(target->cpu))
		return 0;
	if (task_cpu(p) != source->cpu)
		return 0;
	if (task_running(source, p))
		return 0;
	return 1;
}

#define grr_entity_is_task(rt_se) (!(rt_se)->my_q)

static inline struct task_struct *grr_task_of(struct sched_grr_entity *grr_se)
{
/*#ifdef CONFIG_SCHED_DEBUG
	WARN_ON_ONCE(!grr_entity_is_task(grr_se));
#endif*/
	return container_of(grr_se, struct task_struct, grr);
}

static void grr_load_balance(void)
{
	int cpus_online;
	unsigned long i;
	struct load maxload;
	struct load minload;
	struct rq *source_rq;
	struct rq *target_rq;
	struct task_struct *p;
	struct sched_grr_entity *grr_se;

	maxload.nr_running = 0;
	minload.nr_running = 0;

	printk(KERN_ERR "loadbalancing: START\n");

	/*
	 * iterate through each CPU and
	 * find the min and max load accros all CPUs
	 */
	for_each_online_cpu(i) {
		/* get rq of current CPU */
		struct rq *rq = cpu_rq(i);
		/* get grr_rq of current CPU */
		struct grr_rq *grr_rq = &rq->grr;
		/* get nr of running jobs under the GRR policy */
		unsigned long nr_running = grr_rq->grr_nr_running;

		if (maxload.nr_running < nr_running) {
			maxload.nr_running = nr_running;
			maxload.rq = rq;
			maxload.cpu = i;
		}
		if (minload.nr_running > nr_running) {
			minload.nr_running = nr_running;
			minload.rq = rq;
			minload.cpu = i;
		}
		cpus_online++;
	}

	/* given the min and max load
	 * decide if you should load balance
	 */
	if (maxload.nr_running > minload.nr_running+1) {
		/* worth load balancing */
		/* check __migrate_task() from core.c */
		source_rq = maxload.rq;
		target_rq = minload.rq;

		/* lock RQs */
		double_rq_lock(source_rq, target_rq);
		rcu_read_lock();

		/* find next movable task from source_rq */
		list_for_each_entry(grr_se, &source_rq->grr.queue, task_queue) {
			/* get next eligible task from source_rq */
			p = grr_task_of(grr_se);

			if (p == NULL)
				goto unlock;

			if (!can_move_grr_task(p, source_rq, target_rq)) {
				printk(KERN_ERR "could not move task %s from CPU %d to CPU %d\n",
								p->comm,
								source_rq->cpu,
								target_rq->cpu);
				continue;
			}

			/* move task p from source_rq to target_rq
			 * see sched_move_task() in core.c for details
			 */
			deactivate_task(source_rq, p, 0);
			set_task_cpu(p, target_rq->cpu);
			activate_task(target_rq, p, 0);

			printk(KERN_ERR "moved task %s from CPU %d to CPU %d\n",
							p->comm,
							source_rq->cpu,
							target_rq->cpu);

			goto unlock;
		}

		printk(KERN_ERR "no task moved; maxload=%ld, minload=%ld\n",
						maxload.nr_running,
						minload.nr_running);
		goto unlock;
	}
	printk(KERN_ERR "no need to loadbalance; maxload=%ld, minload=%ld\n",
					maxload.nr_running,
					minload.nr_running);
	return;

unlock:
	printk(KERN_ERR "loadbalancing: END\n");
	rcu_read_unlock();
	double_rq_unlock(source_rq, target_rq);
}

void init_grr_rq(struct grr_rq *grr_rq)
{
	printk(KERN_ERR "GRR: init");
	INIT_LIST_HEAD(&grr_rq->queue);
	printk(KERN_ERR "GRR: init DONE");
//	grr_rq->grr_nr_running = 0;
//	raw_spin_lock_init(&grr_rq->grr_runtime_lock);
//	grr_rq->grr_time = 0;
//	grr_rq->grr_throttled = 0;
//	grr_rq->grr_runtime = 0;

}

static void destroy_rt_bandwidth(struct rt_bandwidth *rt_b)
{
}
/*
static inline struct task_struct *grr_task_of(struct sched_grr_entity *grr_se)
{
//#ifdef CONFIG_SCHED_DEBUG
//	WARN_ON_ONCE(!grr_entity_is_task(grr_se));
//#endif
	return container_of(grr_se, struct task_struct, grr);
}

//static inline struct grr_rq *grr_rq_of_se(struct sched_grr_entity *grr_se)
//{
//	return grr_se->grr_rq;
//}
*/
/*
 * Given a runqueue return the running queue of grr policy
 */
static inline struct list_head *grr_queue_of_rq(struct rq *rq)
{
	return &rq->grr.queue;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
static void update_curr_grr(struct rq *rq)
{
	struct task_struct *curr = rq->curr;
	u64 delta_exec;

	//trace_printk("GRR: update_curr_grr\n");
	if (curr->sched_class != &grr_sched_class)
		return;

	delta_exec = rq->clock_task - curr->se.exec_start;
	if (unlikely((s64)delta_exec < 0))
		delta_exec = 0;

	schedstat_set(curr->se.statistics.exec_max,
		      max(curr->se.statistics.exec_max, delta_exec));

	 curr->se.sum_exec_runtime += delta_exec;
	 account_group_exec_runtime(curr, delta_exec);

	 curr->se.exec_start = rq->clock_task;
	 cpuacct_charge(curr, delta_exec);

}

static void dequeue_grr_entity(struct sched_grr_entity *grr_se)
{
	//trace_printk("GRR: dequeue_grr_entity\n");
	/*
	 * Drop connection of this entity with runque but
	 * reinitialize it to be reconnected later.
	 */
	list_del_init(&grr_se->task_queue);
}

static void enqueue_grr_entity(struct rq *rq, struct sched_grr_entity *grr_se, bool head)
{
	//trace_printk("GRR: enqueue_grr_entity\n");
	printk(KERN_ERR "in enqueue entity\n");
	struct list_head *queue = grr_queue_of_rq(rq);

//	struct task_struct *tsk = grr_task_of(grr_se);
//	struct list_head *queue = &tsk->grr.task_queue;

	//struct grr_rq *pgrr_rq = grr_rq_of_se(grr_se);
//	printk(KERN_ERR "grr_rq \n");

//	printk(KERN_ERR "taike head %p\n", grr_se);
//	printk(KERN_ERR "taike head %p\n", tsk);
//	printk(KERN_ERR "taike head %p \n", tsk->grr);
//	printk(KERN_ERR "taike head %p \n", tsk->grr.queue);
//	printk(KERN_ERR "taike head %p \n", queue);
//
	if (head) {
		printk (KERN_ERR "1\n");
		list_add(&grr_se->task_queue, queue);
	}
	else
		list_add_tail(&grr_se->task_queue, queue);
	printk(KERN_ERR "HO:AAAAAAA\n");
}

/*
 * Adding/removing a task to/from a priority array:
 */
static void
enqueue_task_grr(struct rq *rq, struct task_struct *p, int flags)
{
//	trace_printk("GRR: enqueue_task_grr\n");
	printk(KERN_ERR "in enqueue task: %p\n", p);
	printk(KERN_ERR "in enqueue task: %p\n", &(p->grr));
	struct sched_grr_entity *grr_se = &(p->grr);


	enqueue_grr_entity(rq, grr_se, flags & ENQUEUE_HEAD);
	inc_nr_running(rq);
}

static void
dequeue_task_grr(struct rq *rq, struct task_struct *p, int flags)
{
	struct sched_grr_entity *grr_se = &p->grr;

	printk("GRR: dequeue_task_grr\n");
	update_curr_grr(rq);
	dequeue_grr_entity(grr_se);
	dec_nr_running(rq);

}

static void requeue_task_grr(struct rq *rq, struct task_struct *p, int head)
{
	struct sched_grr_entity *grr_se = &p->grr;
	struct grr_rq *grr_rq = &rq->grr;

	//trace_printk("GRR: requeue_task_grr\n");
	list_move_tail(&grr_se->task_queue, &grr_rq->queue);
}

static void yield_task_grr(struct rq *rq)
{
	//trace_printk("GRR: yield_task_grr\n");
	requeue_task_grr(rq,rq->curr,0);
}

static int
select_task_rq_grr(struct task_struct *p, int sd_flag, int flags)
{
	int i;
	int orig_cpu = task_cpu(p);
	struct rq *rq;
	int smallest_rq = orig_cpu;
	unsigned long orig_weight = cpu_rq(orig_cpu)->grr.grr_nr_total;
	unsigned long smallest_rq_weight = orig_weight;

	//trace_printk("GRR: select_task_rq_grr\n");
	if (p->grr.nr_cpus_allowed == 1)
		return orig_cpu;

	rq = cpu_rq(orig_cpu);

	/*
	 * Here load balancing should take place
	 */

	for_each_online_cpu(i) {
		struct grr_rq *grr_rq = &cpu_rq(i)->grr;
		if (!cpumask_test_cpu(i, &p->cpus_allowed))
			continue;
		if (grr_rq->grr_nr_total < smallest_rq_weight) {
			smallest_rq_weight = grr_rq->grr_nr_total;
			smallest_rq = i;
		}
	}
	return smallest_rq;
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
static void check_preempt_curr_grr(struct rq *rq, struct task_struct *p, int flags)
{
	//trace_printk("GRR: check_preempt_curr_grr\n");
	(void)rq;
	(void)p;
	(void)flags;
}

/*
 * As we want a round robin we should put all of our task in a queue.
 * Then the pick_next_task will be just get the head of this list
 */
static struct task_struct *pick_next_task_grr(struct rq *rq)
{
	struct sched_grr_entity *head;
	struct task_struct *p;
	struct grr_rq *grr_rq  = &rq->grr;

	//trace_printk("GRR: pick_next_task_grr\n");
	if (unlikely(!grr_rq->grr_nr_running))
		return NULL;

	head = list_first_entry(&rq->grr.queue, struct sched_grr_entity,
				task_queue);

	p = grr_task_of(head);
	p->se.exec_start = rq->clock;
	return p;
}

static void put_prev_task_grr(struct rq *rq, struct task_struct *p)
{
	//trace_printk("GRR: put_prev_task_grr\n");
	/*
	 * As we are round robin we should put the start
	 * to 0 and update the current task
	 */
	update_curr_grr(rq);
	p->se.exec_start = 0;

}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
static void switched_to_grr(struct rq *rq, struct task_struct *p)
{
	//trace_printk("GRR: switched_to_grr\n");
	/*
	 * If the running proccess in not real time then
	 * this process must execute
	 */
	printk(KERN_ERR "in switch_to\n");
	if (!rt_task(rq->curr))
		resched_task(rq->curr);
	printk(KERN_ERR "exit switch_to");

}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
static void
prio_changed_grr(struct rq *rq, struct task_struct *p, int oldprio)
{
	//trace_printk("GRR: prio_changed_grr\n");
	(void)rq;
	(void)p;
	(void)oldprio;
}

static void watchdog(struct rq *rq, struct task_struct *p)
{
	unsigned long soft, hard;

	//trace_printk("GRR: watchdog\n");
	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	if (soft != RLIM_INFINITY) {
		unsigned long next;

		p->grr.timeout++;
		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		if (p->grr.timeout > next)
			p->cputime_expires.sched_exp = p->se.sum_exec_runtime;
	}
}

static void task_tick_grr(struct rq *rq, struct task_struct *p, int queued)
{
	//trace_printk("GRR: task_tick_grr\n");
	update_curr_grr(rq);

	watchdog(rq, p);

	if (--p->grr.time_slice)
		return;

	p->grr.time_slice = GRR_TIMESLICE;

/*
	 * Requeue to the end of queue if we (and all of our ancestors) are the
	 * only element on the queue

	for_each_sched_entity(grr_se) {
		if (grr_se->run_list.prev != grr_se->run_list.next) {
			requeue_task_grr(rq, p, 0);
			set_tsk_need_resched(p);
			return;
		}
	}
*/
}

static void set_curr_task_grr(struct rq *rq)
{
	printk(KERN_ERR "in set_curr\n");
	trace_printk("GRR: set_curr_task_grr\n");
	struct task_struct *p = rq->curr;

	p->se.exec_start = rq->clock_task;
}

static unsigned int get_rr_interval_grr(struct rq *rq, struct task_struct *task)
{
	//trace_printk("GRR: get_rr_interval_grr");
	return GRR_TIMESLICE;
}


/*
 * All the scheduling class methods:
 */
const struct sched_class grr_sched_class = {
	.next			= &fair_sched_class,
	.enqueue_task		= enqueue_task_grr,
	.dequeue_task		= dequeue_task_grr,
	.yield_task		= yield_task_grr,

	.check_preempt_curr	= check_preempt_curr_grr,

	.pick_next_task		= pick_next_task_grr,
	.put_prev_task		= put_prev_task_grr,

#ifdef CONFIG_SMP
	.select_task_rq		= select_task_rq_grr,
/*	.rq_online		=, */
/*	.rq_offline		=, */
/*	.switched_from		=, */
#endif

	.set_curr_task          = set_curr_task_grr,
	.task_tick		= task_tick_grr,

	.prio_changed		= prio_changed_grr,
	.switched_to		= switched_to_grr,

	.get_rr_interval	= get_rr_interval_grr,

#ifdef CONFIG_GRR_GROUP_SCHED
	.task_move_group	= task_move_group_grr,
#endif
};

#ifdef CONFIG_SCHED_DEBUG
extern void print_grr_rq(struct seq_file *m, int cpu, struct grr_rq *grr_rq);

void print_grr_stats(struct seq_file *m, int cpu)
{
	//trace_printk("GRR: task_tick_grr\n");
}
#endif /* CONFIG_SCHED_DEBUG */
