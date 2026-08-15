# jQuestions

I wouldn't consider this a project. This is more like the product of my curiosity in finding out what is actually happening in the server/backend in general.

I called it **google-form-cheap-copy** while development but later decided to go with much more lamer name.

The code is suboptimal, not organised, has bugs, lives mostly in one file, and could probably break with a little load. There is no framework and no abstractions holding hands.

It's me and C.

## What is jQuestions?

jQuestions is a small survey/questionnaire thing where you can:

* See a list of existing surveys
* Add your own surveys
* View existing surveys
* Share surveys with other people

I've also included a sample **Programming Quiz** for the people who are mad enough to clone this and actually try it.

## Running it

You'll need GCC/Clang, `make`, and a Unix-like system.

```bash
make run
```

Then open:

```text
http://localhost:8080
```

It also attempts to print the available network addresses, so you can open the server from another device on the same network.

## Structure

```text
.
├── data/
│   └── questions/
├── frontEnd/
│   ├── *.html
│   ├── *.js
│   └── style.css
├── main.c
├── Makefile
└── README.md
```

`main.c` is the entire backend.

Sockets, HTTP parsing, routing, file handling, JSON handling, error handling, IP discovery, shutdown handling, and probably a few responsibilities that have absolutely no business being in the same file.

It is what you might call architecturally jquestionable.

`frontEnd/` Some HTML, CSS and JavaScript that talks to the server through the routes implemented in main.c.

`data/` is where the server stores things.

Surveys are saved as JSON files, while index.txt keeps track of the available surveys.

It's not a database.

It is, in fact, files.

## Why C?

Because I wanted to understand what was happening underneath.

Writing a backend using a framework is useful, but it also means a lot of the interesting work has already been abstracted away.

There is no framework. No dependency injection. No enterprise architecture. No microservices. No container orchestration. No seventeen-layer abstraction designed to return a boolean.

## Things that are definitely not production ready

There are quite a few.

For example:

* HTTP parsing is extremely basic.
* Requests are limited by a `1024` byte buffer.
* There is no proper HTTP parser.
* There is no concurrency.
* One connection is handled at a time.
* There is very little input validation.
* Error handling is optimistic at best.
* JSON is treated more like a string than an actual data format.
* File paths are constructed directly from user input.
* There are almost certainly memory-safety issues waiting for their moment.
* There is no authentication or authorization.
* There is no HTTPS.
* Large requests are not properly supported.
* There are assumptions about the exact structure of incoming JSON.
* The server has no concept of keep-alive connections.
* The code is mostly contained in one C file.
* The architecture could generously be described as "whatever seemed reasonable at 2 AM."

So please don't put this behind a public IP and call it your production infrastructure.

I cannot stop you, but I would strongly prefer not to find out what happens.


---

Despite a million things going wrong...

**it works. On a good day.**
