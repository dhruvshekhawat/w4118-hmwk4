/*
 * Round-Robin Scheduling Class (mapped to the SCHED_GRR policy)
 */

#include "sched.h"

#include <linux/slab.h>

#define BASE_WRR_TIME_SLICE (100 * HZ / 1000) /* time slice of 100ms */

static int do_sched_rt_period_timer(struct rt_bandwidth *rt_b, int overrun);

struct rt_bandwidth def_rt_bandwidth;

static enum hrtimer_restart sched_rt_period_timer(struct hrtimer *timer)
{
}

void init_rt_bandwidth(struct rt_bandwidth *rt_b, u64 period, u64 runtime)
{
}

static void start_rt_bandwidth(struct rt_bandwidth *rt_b)
{
}

void init_grr_rq(struct grr_rq *grr_rq, struct rq *rq)
{
	INIT_LIST_HEAD(&grr_rq->queue);
	grr_rq->grr_nr_running = 0;
}

static void destroy_rt_bandwidth(struct rt_bandwidth *rt_b)
{
}

#define rt_entity_is_task(rt_se) (!(rt_se)->my_q)

static inline struct task_struct *grr_task_of(struct sched_grr_entity *grr_se)
{
#ifdef CONFIG_SCHED_DEBUG
	WARN_ON_ONCE(!grr_entity_is_task(grr_se));
#endif
	return container_of(grr_se, struct task_struct, grr);
}

static inline struct rq *rq_of_rt_rq(struct rt_rq *rt_rq)
{
}

static inline struct rt_rq *rt_rq_of_se(struct sched_rt_entity *rt_se)
{
}

void free_rt_sched_group(struct task_group *tg)
{
}

void init_tg_rt_entry(struct task_group *tg, struct rt_rq *rt_rq,
		struct sched_rt_entity *rt_se, int cpu,
		struct sched_rt_entity *parent)
{
}

int alloc_rt_sched_group(struct task_group *tg, struct task_group *parent)
{
}

static inline struct task_struct *rt_task_of(struct sched_rt_entity *rt_se)
{
}

static inline struct rq *rq_of_rt_rq(struct rt_rq *rt_rq)
{
}

static inline struct rt_rq *rt_rq_of_se(struct sched_rt_entity *rt_se)
{
}

void free_rt_sched_group(struct task_group *tg) { }

int alloc_rt_sched_group(struct task_group *tg, struct task_group *parent)
{
}

static inline int rt_overloaded(struct rq *rq)
{
}

static inline void rt_set_overload(struct rq *rq)
{
}

static inline void rt_clear_overload(struct rq *rq)
{
}

static void update_rt_migration(struct rt_rq *rt_rq)
{
}

static void inc_rt_migration(struct sched_rt_entity *rt_se, struct rt_rq *rt_rq)
{
}

static void dec_rt_migration(struct sched_rt_entity *rt_se, struct rt_rq *rt_rq)
{
}

static inline int has_pushable_tasks(struct rq *rq)
{
}

static void enqueue_pushable_task(struct rq *rq, struct task_struct *p)
{
}

static void dequeue_pushable_task(struct rq *rq, struct task_struct *p)
{
}

#else

static inline void dequeue_pushable_task(struct rq *rq, struct task_struct *p)
{
}

static inline
void inc_rt_migration(struct sched_rt_entity *rt_se, struct rt_rq *rt_rq)
{
}

static inline
void dec_rt_migration(struct sched_rt_entity *rt_se, struct rt_rq *rt_rq)
{
}

#endif /* CONFIG_SMP */

static inline int on_rt_rq(struct sched_rt_entity *rt_se)
{
}

#ifdef CONFIG_RT_GROUP_SCHED

static inline u64 sched_rt_runtime(struct rt_rq *rt_rq)
{
}

static inline u64 sched_rt_period(struct rt_rq *rt_rq)
{
}

typedef struct task_group *grr_rq_iter_t;

static inline struct task_group *next_task_group(struct task_group *tg)
{
}

#define for_each_grr_rq(grr_rq, iter, rq)					\
	for (iter = container_of(&task_groups, typeof(*iter), list);	\
		(iter = next_task_group(iter)) &&			\
		(grr_rq = iter->grr_rq[cpu_of(rq)]);)

static inline void list_add_leaf_rt_rq(struct rt_rq *rt_rq)
{
}

static inline void list_del_leaf_rt_rq(struct rt_rq *rt_rq)
{
}

#define for_each_leaf_rt_rq(rt_rq, rq) \
	list_for_each_entry_rcu(rt_rq, &rq->leaf_rt_rq_list, leaf_rt_rq_list)

#define for_each_sched_rt_entity(rt_se) \
	for (; rt_se; rt_se = rt_se->parent)

static inline struct rt_rq *group_rt_rq(struct sched_rt_entity *rt_se)
{
}

static void enqueue_rt_entity(struct sched_rt_entity *rt_se, bool head);
static void dequeue_rt_entity(struct sched_rt_entity *rt_se);

static void sched_rt_rq_enqueue(struct rt_rq *rt_rq)
{
}

static void sched_rt_rq_dequeue(struct rt_rq *rt_rq)
{
}

static inline int rt_rq_throttled(struct rt_rq *rt_rq)
{
}

static int rt_se_boosted(struct sched_rt_entity *rt_se)
{
}

#ifdef CONFIG_SMP
static inline const struct cpumask *sched_rt_period_mask(void)
{
}
#else
static inline const struct cpumask *sched_rt_period_mask(void)
{
}
#endif

static inline
struct rt_rq *sched_rt_period_rt_rq(struct rt_bandwidth *rt_b, int cpu)
{
}

static inline struct rt_bandwidth *sched_rt_bandwidth(struct rt_rq *rt_rq)
{
}

#else /* !CONFIG_RT_GROUP_SCHED */

static inline u64 sched_rt_runtime(struct rt_rq *rt_rq)
{
}

static inline u64 sched_rt_period(struct rt_rq *rt_rq)
{
}

typedef struct rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	for ((void) iter, rt_rq = &rq->rt; rt_rq; rt_rq = NULL)

static inline void list_add_leaf_rt_rq(struct rt_rq *rt_rq)
{
}

static inline void list_del_leaf_rt_rq(struct rt_rq *rt_rq)
{
}

#define for_each_leaf_rt_rq(rt_rq, rq) \
	for (rt_rq = &rq->rt; rt_rq; rt_rq = NULL)

#define for_each_sched_rt_entity(rt_se) \
	for (; rt_se; rt_se = NULL)

static inline struct rt_rq *group_rt_rq(struct sched_rt_entity *rt_se)
{
}

static inline void sched_rt_rq_enqueue(struct rt_rq *rt_rq)
{
}

static inline void sched_rt_rq_dequeue(struct rt_rq *rt_rq)
{
}

static inline int rt_rq_throttled(struct rt_rq *rt_rq)
{
}

static inline const struct cpumask *sched_rt_period_mask(void)
{
}

static inline
struct rt_rq *sched_rt_period_rt_rq(struct rt_bandwidth *rt_b, int cpu)
{
}

static inline struct rt_bandwidth *sched_rt_bandwidth(struct rt_rq *rt_rq)
{
}

#endif /* CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SMP
/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
static int do_balance_runtime(struct rt_rq *rt_rq)
{
}

/*
 * Ensure this RQ takes back all the runtime it lend to its neighbours.
 */
static void __disable_runtime(struct rq *rq)
{
}

static void disable_runtime(struct rq *rq)
{
}

static void __enable_runtime(struct rq *rq)
{
}

static void enable_runtime(struct rq *rq)
{
}

int update_runtime(struct notifier_block *nfb, unsigned long action, void *hcpu)
{
}

static int balance_runtime(struct rt_rq *rt_rq)
{
}

#else /* !CONFIG_SMP */
static inline int balance_runtime(struct rt_rq *rt_rq)
{
}
#endif /* CONFIG_SMP */

static int do_sched_rt_period_timer(struct rt_bandwidth *rt_b, int overrun)
{
}

static inline int rt_se_prio(struct sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
#endif
}

static int sched_rt_runtime_exceeded(struct rt_rq *rt_rq)
{
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
static void update_curr_grr(struct rq *rq)
{
	struct task_struct *curr = rq->curr;
	struct sched_grr_entity *grr_se = &curr->grr;
	struct grr_rq *grr_rq = grr_rq_of_se(grr_se);
	u64 delta_exec;

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

#if defined CONFIG_SMP

static void
inc_rt_prio_smp(struct rt_rq *rt_rq, int prio, int prev_prio)
{
}

static void
dec_rt_prio_smp(struct rt_rq *rt_rq, int prio, int prev_prio)
{
}

#else /* CONFIG_SMP */

static inline
void inc_rt_prio_smp(struct rt_rq *rt_rq, int prio, int prev_prio) {}
static inline
void dec_rt_prio_smp(struct rt_rq *rt_rq, int prio, int prev_prio) {}

#endif /* CONFIG_SMP */

#if defined CONFIG_SMP || defined CONFIG_RT_GROUP_SCHED
static void
inc_rt_prio(struct rt_rq *rt_rq, int prio)
{
}

static void
dec_rt_prio(struct rt_rq *rt_rq, int prio)
{
}

#else

static inline void inc_rt_prio(struct rt_rq *rt_rq, int prio) {}
static inline void dec_rt_prio(struct rt_rq *rt_rq, int prio) {}

#endif /* CONFIG_SMP || CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_RT_GROUP_SCHED

static void
inc_rt_group(struct sched_rt_entity *rt_se, struct rt_rq *rt_rq)
{
}

static void
dec_rt_group(struct sched_rt_entity *rt_se, struct rt_rq *rt_rq)
{
}

#else /* CONFIG_RT_GROUP_SCHED */

static void
inc_rt_group(struct sched_rt_entity *rt_se, struct rt_rq *rt_rq)
{
}

static inline
void dec_rt_group(struct sched_rt_entity *rt_se, struct rt_rq *rt_rq) {}

#endif /* CONFIG_RT_GROUP_SCHED */

static inline
void inc_rt_tasks(struct sched_rt_entity *rt_se, struct rt_rq *rt_rq)
{
}

static inline
void dec_rt_tasks(struct sched_rt_entity *rt_se, struct rt_rq *rt_rq)
{
}

static void dequeue_rt_entity(struct sched_rt_entity *rt_se)
{
	/*
	 * Drop connection of this entity with runque but
	 * reinitialize it to be reconnected later.
	 */
	list_del_init(&grr_se->task_queue);
}

static void enqueue_grr_entity(struct sched_grr_entity *grr_se, bool head)
{
	if (head)
		list_add(&grr_se->task_queue, queue);
	else
		list_add_tail(&grr_se->task_queue, queue);
}

/*
 * Adding/removing a task to/from a priority array:
 */
static void
enqueue_task_grr(struct rq *rq, struct task_struct *p, int flags)
{
	struct sched_grr_entity *grr_se = &p->grr;

	enqueue_grr_entity(grr_se, flags & ENQUEUE_HEAD);
	inc_nr_running(rq);
}

static void
dequeue_task_grr(struct rq *rq, struct task_struct *p)
{
	struct sched_grr_entity *grr_se = &p->grr;

	update_curr_grr(rq);
	dequeue_grr_entity(grr_se);
	dec_nr_running(rq);

}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
static void
requeue_rt_entity(struct rt_rq *rt_rq, struct sched_rt_entity *rt_se, int head)
{
}

static void requeue_task_rt(struct rq *rq, struct task_struct *p, int head)
{
}

static void yield_task_rt(struct rq *rq)
{
}

#ifdef CONFIG_SMP
static int find_lowest_rq(struct task_struct *task);

static int
select_task_rq_rt(struct task_struct *p, int sd_flag, int flags)
{
}

static void check_preempt_equal_prio(struct rq *rq, struct task_struct *p)
{
}

#endif /* CONFIG_SMP */

/*
 * Preempt the current task with a newly woken task if needed:
 */
static void check_preempt_curr_rt(struct rq *rq, struct task_struct *p, int flags)
{
#ifdef CONFIG_SMP
#endif
}

static struct sched_rt_entity *pick_next_rt_entity(struct rq *rq,
						   struct rt_rq *rt_rq)
{
}

static struct task_struct *_pick_next_task_grr(struct rq *rq)
{
	struct sched_grr_entity *grr_se;
	struct task_struct *p;
	struct grr_rq *grr_rq = &rq->grr;

	if (!grr_rq->grr_nr_running)
		return NULL;

	do {
		grr_se = pick_next_entity(grr_rq);
		set_next_entity(grr_rq, grr_se);
		grr_rq = group_grr_rq(grr_se);
	} while (grr_rq);

	p = grr_task_of(grr_se);
	p->se.exec_start = rq->clock_task;

	return p;
}

static struct task_struct *pick_next_task_grr(struct rq *rq)
{
	struct task_struct *p = _pick_next_task_grr(rq);
#ifdef CONFIG_SMP
#endif
}

static void put_prev_task_grr(struct rq *rq, struct task_struct *p)
{
	/*
	 * As we are round robin we should put the start
	 * to 0 and update the current task
	 */
	update_curr_grr();
	p->se.exec_start = 0;

}

#ifdef CONFIG_SMP

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

static int pick_rt_task(struct rq *rq, struct task_struct *p, int cpu)
{
}

/* Return the second highest RT task, NULL otherwise */
static struct task_struct *pick_next_highest_task_rt(struct rq *rq, int cpu)
{
}

static DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

static int find_lowest_rq(struct task_struct *task)
{
}

/* Will lock the rq it finds */
static struct rq *find_lock_lowest_rq(struct task_struct *task, struct rq *rq)
{
}

static struct task_struct *pick_next_pushable_task(struct rq *rq)
{
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
static int push_rt_task(struct rq *rq)
{
}

static void push_rt_tasks(struct rq *rq)
{
}

static int pull_rt_task(struct rq *this_rq)
{
}

static void pre_schedule_rt(struct rq *rq, struct task_struct *prev)
{
}

static void post_schedule_rt(struct rq *rq)
{
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
static void task_woken_rt(struct rq *rq, struct task_struct *p)
{
}

static void set_cpus_allowed_rt(struct task_struct *p,
				const struct cpumask *new_mask)
{
}

/* Assumes rq->lock is held */
static void rq_online_rt(struct rq *rq)
{
}

/* Assumes rq->lock is held */
static void rq_offline_rt(struct rq *rq)
{
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
static void switched_from_rt(struct rq *rq, struct task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
}

void init_sched_rt_class(void)
{
}
#endif /* CONFIG_SMP */

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
static void switched_to_rt(struct rq *rq, struct task_struct *p)
{
	(void)rq;
	(void)p;
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
static void
prio_changed_grr(struct rq *rq, struct task_struct *p, int oldprio)
{
	(void)rq;
	(void)p;
	(void)oldprio;
}

static void watchdog(struct rq *rq, struct task_struct *p)
{
	unsigned long soft, hard;

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
	struct sched_grr_entity *grr_se = &p->grr;

	update_curr_grr(rq);

	watchdog(rq, p);

	if (--p->grr.time_slice)
		return;

	p->grr.time_slice = GRR_TIMESLICE;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are the
	 * only element on the queue
	 */
	for_each_sched__entity(grr_se) {
		if (grr_se->run_list.prev != grr_se->run_list.next) {
			requeue_task_grr(rq, p, 0);
			set_tsk_need_resched(p);
			return;
		}
	}
}

static void set_curr_task_grr(struct rq *rq)
{
	struct task_struct *p = rq->curr;

	p->se.exec_start = rq->clock_task;

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);
}

static unsigned int get_rr_interval_grr(struct rq *rq, struct task_struct *task)
{
	return GRR_TIMESLICE;
}


/*
 * All the scheduling class methods:
 */
const struct sched_class sched_grr_class = {
	.next			= &fair_sched_class,
	.enqueue_task		= enqueue_task_grr,
	.dequeue_task		= dequeue_task_grr,
	.yield_task		=,

	.check_preempt_curr	=,

	.pick_next_task		= pick_next_task_grr,
	.put_prev_task		= put_prev_task_grr,

#ifdef CONFIG_SMP
	.select_task_rq		=,
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
	grr_rq_iter_t iter;
	struct grr_rq *grr_rq;

	rcu_read_lock();
	for_each_grr_rq(grr_rq, iter, cpu_rq(cpu))
		print_grr_rq(m, cpu, grr_rq);
	rcu_read_unlock();

}
#endif /* CONFIG_SCHED_DEBUG */

struct load {
	struct rq *rq;
	int value;
};

static void grr_load_balance(void)
{
	unsigned long i;
	unsigned long online_cpus;
	struct load maxload;
	struct load minload;

	maxload.value = 0;
	minload.value = 0;

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

		if (maxload.value < nr_running) {
			maxload.value = nr_running;
			maxload.rq = grr_rq;
		}
		if (minload.value > nr_running) {
			minload.value = nr_running;
			minload.rq = grr_rq;
		}
		online_cpus++;
	}

	/* given the min and max load
	 * decide if you should load balance
	 */
	if (maxload.value > minload.value+1) {
		/* worth load balancing */
		struct rq *source_rq = maxload.rq;
		struct rq *target_rq = minload.rq;

		/* get next eligible task from source_rq */
		struct task_struct *p = get_next_task(); // TODO: fill in correct func

		/* move task p from source_rq to target_rq
		 * see sched_move_task() in core.c for details
		 */
		deactivate_task(source_rq, p, 0);
		set_task_cpu(p, target_rq->cpu);
		actovate_task(target_rq, p, 0);
	} else {
		return;
	}
}
