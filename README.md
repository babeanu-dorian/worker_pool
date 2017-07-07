[![Build Status](https://travis-ci.org/babeanu-dorian/worker_pool.svg?branch=master)](https://travis-ci.org/babeanu-dorian/worker_pool)
[![Coverage Status](https://coveralls.io/repos/github/babeanu-dorian/worker_pool/badge.svg?branch=master)](https://coveralls.io/github/babeanu-dorian/worker_pool?branch=master)

# worker_pool
A group of threads executing tasks from a queue.

#Classes in the wp namespace:
..* worker_pool
..* priority_task
..* priority_task_queue

#worker_pool:

|Template Arguments| | | |
|:----:|:----:|:----:|:----:|
|Name|Description|Requirements|Default|
|:---:|:---:|:---:|:---:|
|Functor|The type of a task (function pointer, callable etc.)|..* callable with 0 parameters <br> ..* move contructible <br> ..* move assignable| - |
|Container|The container used to store the tasks|..* container <br> ..*element type is Functor <br> ..* allows move insertion <br> ..* allows access to at least one element through a non-const reference <br> ..* allows deletion of the accessible element| std::queue< Functor, std::list<Functor> >|
