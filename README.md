[![Build Status](https://travis-ci.org/babeanu-dorian/worker_pool.svg?branch=master)](https://travis-ci.org/babeanu-dorian/worker_pool)
[![Coverage Status](https://coveralls.io/repos/github/babeanu-dorian/worker_pool/badge.svg?branch=master)](https://coveralls.io/github/babeanu-dorian/worker_pool?branch=master)

# worker_pool
A group of threads executing tasks from a queue.

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
* void worker_count(size_t nWorkers);
