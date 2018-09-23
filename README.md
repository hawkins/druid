# druid
A systems-level programming language with a focus on integrated build systems and deploying to native and web assembly at once


## Rationale

We at Druid love C, but we have a few qualms with C. These include things like

- `->` instead of `.` for pointer member accessing
- Things too quickly decay to pointers. E.g., `struct` parameter & `sizeof`
- Lack of a first-class string library
- No first-party build system or IDE integrations
- Enum namespacing. E.g., `Enum::DrawState::unset` clashes with `Enum::ErrorState::unset`

There are some other awesome things we want to add to C, like

- Compiler logs of what was optimized / inlined and logs of when something was almost able to be optimized but a malpractice prevented this.
  - We <3 compiler optimizations, but sometimes you thing you're using them when your code is actually inelligible. Let's make logs and show the users when they *think* they're using optimizations, but aren't quite.
- Friendly compiler that interactively compiles, i.e., shows you compilation steps with optimizations, warnings, and errors in real time.
- And more cool digs to come!

## The Story

[@hawkins](https://github.com/hawkins) and [@assemblyDruid](https://github.com/assemblyDruid) plan to develop Druid nearly 100% on recording or live streamed. Check out [Twitch.tv/druidlang](https://twitch.tv/druidlang) to watch all of our developments :) Druid started from a desire to learn more about compiler toolchain development, but we hope it will grow into a realistic systems-level programming language for use in the future.
