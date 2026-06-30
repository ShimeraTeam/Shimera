# Contributing to Shimera

Thanks for taking the time to contribute. Whether you're fixing a bug, adding a new feature, improving documentation, or just giving feedback, your contribution is valuable to the project. This guide will help you understand how to contribute effectively and ensure that your changes can be easily reviewed and merged. Small improvements are welcome as much as larger features.

## Philosophy

Shimera exists to solve a single problem: **users should need only one call to display a shader**, regardless of whether they use SFML, Raylib or pure OpenGL.

To achieve this, we follow these core principles:

- **Simplify, don't expose.** Hide low-level OpenGL and backend-specific complexity behind clean interfaces.
- **Consistency across backends.** The same code should work the same way on SFML, Raylib and OpenGL.
- **Keep interfaces minimal.** Add only what is necessary to solve the user's problem.
- **Support effect chaining.** Multiple effects should work together cleanly.
- **Performance**: We care about the performance of the library and its users' applications.
- **Documentation**: We want to make it easy for users to understand how to use the library and for contributors to understand how it works.

When you contribute, ask yourself: *Does this change make it easier for users to display shaders? Does it keep the library consistent and maintainable?*

## Before You Start

- Read the current README, the documentation, and the relevant backend or effect code before changing anything.
- Keep changes scoped to a single goal when possible.
- Open an issue first if you want to discuss a larger feature or a breaking change.

#### Some interesting reads before you start:
- [Shimera's README](README.md) - for an overview of the project and its goals.
- [Shimera's License](LICENSE) - to understand the terms under which you are contributing.
- [Shimera developer documentation](https://shimerateam.github.io/ShimeraDocs/devdoc/) - for in-depth technical details and guidelines.
- [Shimera user documentation](https://shimerateam.github.io/ShimeraDocs/userdoc/) - for examples and user-facing documentation.

If you prefer to read the docs locally, see the `ShimeraDocs/public/docs` folder for the source content.

## Licensing of Contributions

Shimera is released under the **GNU General Public License v3.0** (`GPL-3.0-only`); see the [LICENSE](LICENSE) file.

By submitting a contribution (a pull request, patch, or any other form of change), you certify that:

- you wrote the contribution yourself, or otherwise have the right to submit it under the project's license;
- your contribution is provided under the terms of the GPL-3.0; and
- you grant the Shimera maintainers (the people listed in the **Maintainers** section of [AUTHORS.md](AUTHORS.md)) a perpetual, worldwide, non-exclusive, royalty-free, irrevocable license to use, reproduce, modify, sublicense and distribute your contribution, **including the right to relicense it, together with the rest of the project, under any other license in the future, whether open source or proprietary/commercial** (for example to dual-license the project as GPL-3.0 plus a commercial license, or to move it to a more permissive open-source license such as MPL-2.0).

This grant lets the project adapt its license over time and pursue dual-licensing while keeping every contribution usable. The license you grant is **non-exclusive**: you keep full copyright to your own contributions and may use them elsewhere as you wish.

New source files should start with the standard license header used across the codebase (`SPDX-License-Identifier: GPL-3.0-only` and the `The Shimera Authors` copyright line).

## Code Style

- Follow the existing style in the file you are editing.
- Prefer clear names and small functions.
- Keep public headers minimal and stable.
- Avoid unrelated formatting changes.
- If a change affects multiple backends, keep the behavior consistent across them.

For more information on code standards and practices, see the [Coding Standards](https://shimerateam.github.io/ShimeraDocs/devdoc/code_standards/) and the [Git Workflow](https://shimerateam.github.io/ShimeraDocs/devdoc/git_workflow/) documentation.

## Testing

Before opening a pull request, make sure the project still builds and, when relevant, run the example or memory test targets for the backend you touched.

If your change affects rendering, shaders, or backend behavior, add or update an example when possible.

## Documentation

If you change a public API, build step, or user-facing behavior, update the README or the documentation alongside the code.

For docs work, keep the English and French versions in sync when the change applies to both.

## Pull Requests

Good pull requests are:

- focused on one topic,
- easy to understand from the description,
- backed by a successful build or relevant test run,
- and accompanied by screenshots or logs when they help explain the change.

Please mention the backend you changed, the commands you ran, and any known limitations.

## Community

Be respectful and constructive in issues and reviews. Contributions should make the project easier to use, easier to maintain, or easier to understand.
