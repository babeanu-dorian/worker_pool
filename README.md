[![Build Status](https://travis-ci.org/babeanu-dorian/worker_pool.svg?branch=master)](https://travis-ci.org/babeanu-dorian/worker_pool)
[![Coverage Status](https://coveralls.io/repos/github/babeanu-dorian/worker_pool/badge.svg??branch=master)](https://coveralls.io/github/babeanu-dorian/worker_pool?branch=master)

# worker_pool
A group of threads executing tasks from a container. The user has control over the number of threads in the pool, container type and behaviour, and the threads' workflow.

# Classes in the wp namespace:
* worker_pool
* priority_task
* priority_task_queue

# worker_pool:
<table align="center">
  <tr>
    <th>Template Argument</th>
    <th>Description</th>
    <th>Requirements</th>
    <th>Default</th>
  </tr>
  <tr>
    <td>Functor</td>
    <td>The type of a task (function pointer, callable etc.)</td>
    <td>
      <ul>
        <li>callable with 0 parameters</li>
        <li>move contructible</li>
        <li>move assignable</li>
      </ul>
    <td> - </td>
  </tr>
  <tr>
    <td>Container</td>
    <td>The container used to store the tasks</td>
    <td>
      <ul>
        <li>container</li>
        <li>element type is Functor</li>
        <li>allows move insertion</li>
        <li>container</li>
        <li>element type is Functor</li>
        <li>allows move insertion</li>
        <li>allows access to at least one element through a non-const reference</li>
        <li>allows removal of the accessible element</li>
        <li>has a method <i>size_t size(void)</i> which returns the number of stored elements</li>
        <li>has a method <i>bool empty(void)</i> which returns whether the container is empty or not</li>
      </ul>
    <td> std::queue &lt; Functor, std::list &lt; Functor &gt; &gt; </td>
  </tr>
  <tr>
    <td>Next</td>
    <td>Method for accessing a task in the container</td>
    <td>
      <ul>
        <li>member function of Container</li>
        <li>empty parameter list</li>
        <li>returns an lvalue reference of type Functor</li>
      </ul>
    <td> &amp;Container::front </td>
  </tr>
  <tr>
    <td>Pop</td>
    <td>Method for removing the element accessed through Next</td>
    <td>
      <ul>
        <li>member function of Container</li>
        <li>empty parameter list</li>
        <li>returns void</li>
      </ul>
    <td> &amp;Container::pop </td>
  </tr>
  <tr>
    <td>Push</td>
    <td>Method for inserting a task into the container</td>
    <td>
      <ul>
        <li>member function of Container</li>
        <li>takes an rvalue reference of type Functor as parameter</li>
        <li>returns void</li>
      </ul>
    <td> &amp;Container::push </td>
  </tr>
</table>

Public interface:
* worker_pool(size_t nWorkers, Container &&container = Container());<br>
Constructor.<br>
Parameters:
  * nWorkers - number of worker threads
  * container - a container with tasks which will be moved and used as the underlaying task container
* Deleted copy and move constructors and assignment operators.
* void push(Functor &&task);<br>
Enqueue a task.<br>
Parameters:
  * task - the task of type Functor to be enqueued by moving
* template &lt;typename InputI&gt;<br>void push(InputIt first, InputIt last);<br>
Enqueue all tasks in the interval [first, last).<br>
Parameters:
  * first - iterator to the first task to be enqueued by moving
  * last - iterator to the element after the last task to be enqueued by moving
* void worker_count(size_t nWorkers);<br>
Change the number of workers in the pool. When the number is decreased, the call to this function will block until the retired threads finish their current tasks.
Parameters:
  * nWorkers - the new number of worker threads in the pool
* size_t worker_count();<br>
Returns the number of worker threads in the pool.
* size_t task_count();<br>
Returns the number of tasks in the queue.
* void pause();<br>
Prevents the workers from popping tasks from the queue. Workers will finish the already popped tasks before pausing.
* void resume();<br>
Allows worker threads to pop tasks from the queue.
* bool is_paused();<br>
Returns true if the worker threads are not allowed to pop tasks from the queue, false otherwise.
* void wait_finish();<br>
Blocks until there are no tasks left in the queue and all the worker threads are waiting for new ones.
* Non-virtual destructor.

# priority_task:

A wrapper that allows pairing a task (callable type) with a priority (size_t).

<table align="center">
  <tr>
    <th>Template Argument</th>
    <th>Description</th>
    <th>Requirements</th>
    <th>Default</th>
  </tr>
  <tr>
    <td>Functor</td>
    <td>The type of a task (function pointer, callable etc.)</td>
    <td>
      <ul>
        <li>callable with 0 parameters</li>
        <li>move contructible</li>
        <li>move assignable</li>
      </ul>
    <td> - </td>
  </tr>
</table>

Public interface:
* priority_task(Functor &&func, size_t priority = 0);<br>
Contructor.<br>
Parameters:
  * func - task to be moved and stored by the priority_task
  * priority - the initial priority of the task, used to order the tasks in a priority queue
* void priority(size_t prio);<br>
Change the priority of the task.<br>
Parameters:
  * prio - new priority of the task
* size_t priority();<br>
Returns the current priority of the task.
* void operator();<br>
Calls the stored task function.
* Implicitly declared destructor, copy and move constructors and assignment operators.
* template &lt;typename T&gt;<br>friend bool operator&lt;(priority_task&lt;T&gt; const &task1, priority_task&lt;T&gt; const &task2);<br>
Comparison operator used by priority queues to sort the tasks based on their priority.

# priority_task_queue

A wrapper around std::priority_queue that can be used as a container for wp::worker_pool. Basically removes the const restriction from the return of std::priority_queue::top. 

<table align="center">
  <tr>
    <th>Template Argument</th>
    <th>Description</th>
    <th>Requirements</th>
    <th>Default</th>
  </tr>
  <tr>
    <td>Functor</td>
    <td>The type of a task (function pointer, callable etc.)</td>
    <td>
      <ul>
        <li>all element type restrictions of std::priority_queue</li>
        <li>move contructible</li>
        <li>move assignable</li>
      </ul>
    <td> - </td>
  </tr>
  <tr>
    <td>...Args</td>
    <td>Additional template arguments needed by std::priority_queue.</td>
    <td>
      Restrictions imposed by std::priority_queue. 
    <td> Defaults provided by std::priority_queue. </td>
  </tr>
</table>

Public interface:
* template &lt;typename ...Params&gt;<br>priority_task_queue(Params&&... params);<br>
Constructor. Forwards its parameters to the constructor of std::priority_queue.
* size_t size();<br>
Returns the number of elements in the queue.
* bool empty();<br>
Returns true if the queue is empty, false otherwise.
* Functor &front();<br>
Returns a non-const reference to the first element in the underlying std::priority_queue, only use for moving the element before popping it.
* void pop();<br>
Removes the top element from the queue.
* void push(Functor &&value);<br>
Inserts an element into the queue.<br>
Parameters:
  * value - task to be moved into the queue
  
