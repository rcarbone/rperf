/* The MIT License

   Copyright (C) 2011 Zilong Tan (eric.zltan@gmail.com)

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

/* This implementation is based on the one from Linux kernel. */

#ifndef _ULIB_LIST_H
#define _ULIB_LIST_H

#include <stddef.h>

#ifdef __cplusplus
#define new _new_
#endif

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct ulist_head {
	struct ulist_head *next, *prev;
};

struct hlist_head {
	struct hlist_node *first;
};

struct hlist_node {
	struct hlist_node *next, **pprev;
};

#define ULIST_HEAD_INIT(name) { &(name), &(name) }

#define ULIST_HEAD(name)				\
	struct ulist_head name = ULIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct ulist_head *list)
{
	list->next = list;
	list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct ulist_head *new,
			      struct ulist_head *prev,
			      struct ulist_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * ulist_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void ulist_add(struct ulist_head *new, struct ulist_head *head)
{
	__list_add(new, head, head->next);
}


/**
 * ulist_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void ulist_add_tail(struct ulist_head *new, struct ulist_head *head)
{
	__list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct ulist_head * prev, struct ulist_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * ulist_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: ulist_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void __list_del_entry(struct ulist_head *entry)
{
	__list_del(entry->prev, entry->next);
}

static inline void ulist_del(struct ulist_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = 0;
	entry->prev = 0;
}

/**
 * ulist_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static inline void ulist_replace(struct ulist_head *old,
				struct ulist_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

static inline void ulist_replace_init(struct ulist_head *old,
				     struct ulist_head *new)
{
	ulist_replace(old, new);
	INIT_LIST_HEAD(old);
}

/**
 * ulist_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void ulist_del_init(struct ulist_head *entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}

/**
 * ulist_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void ulist_move(struct ulist_head *list, struct ulist_head *head)
{
	__list_del_entry(list);
	ulist_add(list, head);
}

/**
 * ulist_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void ulist_move_tail(struct ulist_head *list,
				  struct ulist_head *head)
{
	__list_del_entry(list);
	ulist_add_tail(list, head);
}

/**
 * ulist_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int ulist_is_last(const struct ulist_head *list,
			       const struct ulist_head *head)
{
	return list->next == head;
}

/**
 * ulist_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int ulist_empty(const struct ulist_head *head)
{
	return head->next == head;
}

/**
 * ulist_empty_careful - tests whether a list is empty and not being modified
 * @head: the list to test
 *
 * Description:
 * tests whether a list is empty _and_ checks that no other CPU might be
 * in the process of modifying either member (next or prev)
 *
 * NOTE: using ulist_empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is ulist_del_init(). Eg. it cannot be used
 * if another CPU could re-list_add() it.
 */
static inline int ulist_empty_careful(const struct ulist_head *head)
{
	struct ulist_head *next = head->next;
	return (next == head) && (next == head->prev);
}

/**
 * ulist_rotate_left - rotate the list to the left
 * @head: the head of the list
 */
static inline void ulist_rotate_left(struct ulist_head *head)
{
	struct ulist_head *first;

	if (!ulist_empty(head)) {
		first = head->next;
		ulist_move_tail(first, head);
	}
}

/**
 * ulist_is_singular - tests whether a list has just one entry.
 * @head: the list to test.
 */
static inline int ulist_is_singular(const struct ulist_head *head)
{
	return !ulist_empty(head) && (head->next == head->prev);
}

static inline void __list_cut_position(struct ulist_head *list,
				       struct ulist_head *head, struct ulist_head *entry)
{
	struct ulist_head *new_first = entry->next;
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry;
	entry->next = list;
	head->next = new_first;
	new_first->prev = head;
}

/**
 * ulist_cut_position - cut a list into two
 * @list: a new list to add all removed entries
 * @head: a list with entries
 * @entry: an entry within head, could be the head itself
 *	and if so we won't cut the list
 *
 * This helper moves the initial part of @head, up to and
 * including @entry, from @head to @list. You should
 * pass on @entry an element you know is on @head. @list
 * should be an empty list or a list you do not care about
 * losing its data.
 *
 */
static inline void ulist_cut_position(struct ulist_head *list,
				     struct ulist_head *head, struct ulist_head *entry)
{
	if (ulist_empty(head))
		return;
	if (ulist_is_singular(head) &&
	    (head->next != entry && head != entry))
		return;
	if (entry == head)
		INIT_LIST_HEAD(list);
	else
		__list_cut_position(list, head, entry);
}

static inline void __list_splice(const struct ulist_head *list,
				 struct ulist_head *prev,
				 struct ulist_head *next)
{
	struct ulist_head *first = list->next;
	struct ulist_head *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

/**
 * ulist_splice - join two lists, this is designed for stacks
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void ulist_splice(const struct ulist_head *list,
			       struct ulist_head *head)
{
	if (!ulist_empty(list))
		__list_splice(list, head, head->next);
}

/**
 * ulist_splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void ulist_splice_tail(struct ulist_head *list,
				    struct ulist_head *head)
{
	if (!ulist_empty(list))
		__list_splice(list, head->prev, head);
}

/**
 * ulist_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void ulist_splice_init(struct ulist_head *list,
				    struct ulist_head *head)
{
	if (!ulist_empty(list)) {
		__list_splice(list, head, head->next);
		INIT_LIST_HEAD(list);
	}
}

/**
 * ulist_splice_tail_init - join two lists and reinitialise the emptied list
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
static inline void ulist_splice_tail_init(struct ulist_head *list,
					 struct ulist_head *head)
{
	if (!ulist_empty(list)) {
		__list_splice(list, head->prev, head);
		INIT_LIST_HEAD(list);
	}
}

#define prefetch(x) __builtin_prefetch(x)

/**
 * ulist_entry - get the struct for this entry
 * @ptr:	the &struct ulist_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the ulist_struct within the struct.
 */
#define ulist_entry(ptr, type, member)					\
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * ulist_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the ulist_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define ulist_first_entry(ptr, type, member)	\
	ulist_entry((ptr)->next, type, member)

/**
 * ulist_for_each	-	iterate over a list
 * @pos:	the &struct ulist_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define ulist_for_each(pos, head)					\
	for (pos = (head)->next; prefetch(pos->next), pos != (head);	\
	     pos = pos->next)

/**
 * __list_for_each	-	iterate over a list
 * @pos:	the &struct ulist_head to use as a loop cursor.
 * @head:	the head for your list.
 *
 * This variant differs from list_for_each() in that it's the
 * simplest possible list iteration code, no prefetching is done.
 * Use this for code that knows the list to be very short (empty
 * or 1 entry) most of the time.
 */
#define __list_for_each(pos, head)					\
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * ulist_for_each_prev	-	iterate over a list backwards
 * @pos:	the &struct ulist_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define ulist_for_each_prev(pos, head)					\
	for (pos = (head)->prev; prefetch(pos->prev), pos != (head);	\
	     pos = pos->prev)

/**
 * ulist_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the &struct ulist_head to use as a loop cursor.
 * @n:		another &struct ulist_head to use as temporary storage
 * @head:	the head for your list.
 */
#define ulist_for_each_safe(pos, n, head)			\
	for (pos = (head)->next, n = pos->next; pos != (head);	\
	     pos = n, n = pos->next)

/**
 * ulist_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
 * @pos:	the &struct ulist_head to use as a loop cursor.
 * @n:		another &struct ulist_head to use as temporary storage
 * @head:	the head for your list.
 */
#define ulist_for_each_prev_safe(pos, n, head)		\
	for (pos = (head)->prev, n = pos->prev;		\
	     prefetch(pos->prev), pos != (head);	\
	     pos = n, n = pos->prev)

/**
 * ulist_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the ulist_struct within the struct.
 */
#define ulist_for_each_entry(pos, head, member)				\
	for (pos = ulist_entry((head)->next, typeof(*pos), member);	\
	     prefetch(pos->member.next), &pos->member != (head);	\
	     pos = ulist_entry(pos->member.next, typeof(*pos), member))

/**
 * ulist_for_each_entry_reverse - iterate backwards over list of given type.
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the ulist_struct within the struct.
 */
#define ulist_for_each_entry_reverse(pos, head, member)			\
	for (pos = ulist_entry((head)->prev, typeof(*pos), member);	\
	     prefetch(pos->member.prev), &pos->member != (head);	\
	     pos = ulist_entry(pos->member.prev, typeof(*pos), member))

/**
 * ulist_prepare_entry - prepare a pos entry for use in ulist_for_each_entry_continue()
 * @pos:	the type * to use as a start point
 * @head:	the head of the list
 * @member:	the name of the ulist_struct within the struct.
 *
 * Prepares a pos entry for use as a start point in ulist_for_each_entry_continue().
 */
#define ulist_prepare_entry(pos, head, member)			\
	((pos) ? : ulist_entry(head, typeof(*pos), member))

/**
 * ulist_for_each_entry_continue - continue iteration over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the ulist_struct within the struct.
 *
 * Continue to iterate over list of given type, continuing after
 * the current position.
 */
#define ulist_for_each_entry_continue(pos, head, member)			\
	for (pos = ulist_entry(pos->member.next, typeof(*pos), member);	\
	     prefetch(pos->member.next), &pos->member != (head);	\
	     pos = ulist_entry(pos->member.next, typeof(*pos), member))

/**
 * ulist_for_each_entry_continue_reverse - iterate backwards from the given point
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the ulist_struct within the struct.
 *
 * Start to iterate over list of given type backwards, continuing after
 * the current position.
 */
#define ulist_for_each_entry_continue_reverse(pos, head, member)		\
	for (pos = ulist_entry(pos->member.prev, typeof(*pos), member);	\
	     prefetch(pos->member.prev), &pos->member != (head);	\
	     pos = ulist_entry(pos->member.prev, typeof(*pos), member))

/**
 * ulist_for_each_entry_from - iterate over list of given type from the current point
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the ulist_struct within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */
#define ulist_for_each_entry_from(pos, head, member)			\
	for (; prefetch(pos->member.next), &pos->member != (head);	\
	     pos = ulist_entry(pos->member.next, typeof(*pos), member))

/**
 * ulist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the ulist_struct within the struct.
 */
#define ulist_for_each_entry_safe(pos, n, head, member)			\
	for (pos = ulist_entry((head)->next, typeof(*pos), member),	\
		     n = ulist_entry(pos->member.next, typeof(*pos), member); \
	     &pos->member != (head);					\
	     pos = n, n = ulist_entry(n->member.next, typeof(*n), member))

/**
 * ulist_for_each_entry_safe_continue - continue list iteration safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the ulist_struct within the struct.
 *
 * Iterate over list of given type, continuing after current point,
 * safe against removal of list entry.
 */
#define ulist_for_each_entry_safe_continue(pos, n, head, member)		\
		for (pos = ulist_entry(pos->member.next, typeof(*pos), member), \
			     n = ulist_entry(pos->member.next, typeof(*pos), member); \
		     &pos->member != (head);				\
		     pos = n, n = ulist_entry(n->member.next, typeof(*n), member))

/**
 * ulist_for_each_entry_safe_from - iterate over list from current point safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the ulist_struct within the struct.
 *
 * Iterate over list of given type from current point, safe against
 * removal of list entry.
 */
#define ulist_for_each_entry_safe_from(pos, n, head, member)		\
	for (n = ulist_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = n, n = ulist_entry(n->member.next, typeof(*n), member))

/**
 * ulist_for_each_entry_safe_reverse - iterate backwards over list safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the ulist_struct within the struct.
 *
 * Iterate backwards over list of given type, safe against removal
 * of list entry.
 */
#define ulist_for_each_entry_safe_reverse(pos, n, head, member)		\
	for (pos = ulist_entry((head)->prev, typeof(*pos), member),	\
		     n = ulist_entry(pos->member.prev, typeof(*pos), member); \
	     &pos->member != (head);					\
	     pos = n, n = ulist_entry(n->member.prev, typeof(*n), member))

/**
 * ulist_safe_reset_next - reset a stale ulist_for_each_entry_safe loop
 * @pos:	the loop cursor used in the ulist_for_each_entry_safe loop
 * @n:		temporary storage used in ulist_for_each_entry_safe
 * @member:	the name of the ulist_struct within the struct.
 *
 * ulist_safe_reset_next is not safe to use in general if the list may be
 * modified concurrently (eg. the lock is dropped in the loop body). An
 * exception to this is if the cursor element (pos) is pinned in the list,
 * and ulist_safe_reset_next is called after re-taking the lock and before
 * completing the current iteration of the loop body.
 */
#define ulist_safe_reset_next(pos, n, member)			\
	n = ulist_entry(pos->member.next, typeof(*pos), member)

/*
 * Double linked lists with a single pointer list head.
 * Mostly useful for hash tables where the two pointer list head is
 * too wasteful.
 * You lose the ability to access the tail in O(1).
 */

#define HLIST_HEAD_INIT { .first = 0 }
#define HLIST_HEAD(name) struct hlist_head name = {  .first = 0 }
#define INIT_HLIST_HEAD(ptr) ((ptr)->first = 0)
static inline void INIT_HLIST_NODE(struct hlist_node *h)
{
	h->next = 0;
	h->pprev = 0;
}

static inline int hlist_unhashed(const struct hlist_node *h)
{
	return !h->pprev;
}

static inline int hlist_empty(const struct hlist_head *h)
{
	return !h->first;
}

static inline void __hlist_del(struct hlist_node *n)
{
	struct hlist_node *next = n->next;
	struct hlist_node **pprev = n->pprev;
	*pprev = next;
	if (next)
		next->pprev = pprev;
}

static inline void hlist_del(struct hlist_node *n)
{
	__hlist_del(n);
	n->next = 0;
	n->pprev = 0;
}

static inline void hlist_del_init(struct hlist_node *n)
{
	if (!hlist_unhashed(n)) {
		__hlist_del(n);
		INIT_HLIST_NODE(n);
	}
}

static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
	struct hlist_node *first = h->first;
	n->next = first;
	if (first)
		first->pprev = &n->next;
	h->first = n;
	n->pprev = &h->first;
}

/* next must be != 0 */
static inline void hlist_add_before(struct hlist_node *n,
				    struct hlist_node *next)
{
	n->pprev = next->pprev;
	n->next = next;
	next->pprev = &n->next;
	*(n->pprev) = n;
}

static inline void hlist_add_after(struct hlist_node *n,
				   struct hlist_node *next)
{
	next->next = n->next;
	n->next = next;
	next->pprev = &n->next;

	if(next->next)
		next->next->pprev  = &next->next;
}

/* after that we'll appear to be on some hlist and hlist_del will work */
static inline void hlist_add_fake(struct hlist_node *n)
{
	n->pprev = &n->next;
}

/*
 * Move a list from one list head to another. Fixup the pprev
 * reference of the first entry if it exists.
 */
static inline void hlist_move_list(struct hlist_head *old,
				   struct hlist_head *new)
{
	new->first = old->first;
	if (new->first)
		new->first->pprev = &new->first;
	old->first = 0;
}

#define hlist_entry(ptr, type, member)		\
	ulist_entry(ptr,type,member)

#define hlist_for_each(pos, head)					\
	for (pos = (head)->first; pos && ({ prefetch(pos->next); 1; }); \
	     pos = pos->next)

#define hlist_for_each_safe(pos, n, head)				\
	for (pos = (head)->first; pos && ({ n = pos->next; 1; });	\
	     pos = n)

/**
 * hlist_for_each_entry	- iterate over list of given type
 * @tpos:	the type * to use as a loop cursor.
 * @pos:	the &struct hlist_node to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry(tpos, pos, head, member)			\
	for (pos = (head)->first;					\
	     pos && ({ prefetch(pos->next); 1;}) &&			\
		     ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
	     pos = pos->next)

/**
 * hlist_for_each_entry_continue - iterate over a hlist continuing after current point
 * @tpos:	the type * to use as a loop cursor.
 * @pos:	the &struct hlist_node to use as a loop cursor.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_continue(tpos, pos, member)		\
	for (pos = (pos)->next;						\
	     pos && ({ prefetch(pos->next); 1;}) &&			\
		     ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
	     pos = pos->next)

/**
 * hlist_for_each_entry_from - iterate over a hlist continuing from current point
 * @tpos:	the type * to use as a loop cursor.
 * @pos:	the &struct hlist_node to use as a loop cursor.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_from(tpos, pos, member)			\
	for (; pos && ({ prefetch(pos->next); 1;}) &&			\
		     ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
	     pos = pos->next)

/**
 * hlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @tpos:	the type * to use as a loop cursor.
 * @pos:	the &struct hlist_node to use as a loop cursor.
 * @n:		another &struct hlist_node to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_safe(tpos, pos, n, head, member)		\
	for (pos = (head)->first;					\
	     pos && ({ n = pos->next; 1; }) &&				\
		     ({ tpos = hlist_entry(pos, typeof(*tpos), member); 1;}); \
	     pos = n)

#ifdef __cplusplus
#undef new
#endif

#endif
