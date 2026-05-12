# CONTRIBUTING
> all source code files must end with license and copyright notices to ensure compliance with the GNU Affero General Public License v3.0 or later

## Contributor
> anyone who submits source code changes for inclusion by the Licensor

### Modification
> changing existing files

- purely cosmetic: nothing needs to be done

- small changes andor additions: add modification notices near such changes

- significant changes: add your copyright notice on a new line at the end of other copyright notices

### Creation
> adding new files

add similar comment matching the Licensor template, but include an additional empty line between the `app:` line and your `copyright` notice (`DO NOT USE LICENSOR COPYRIGHT NOTICE`), like so:

```md
GNU Affero General Public License v3.0 or later
NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
SPDX-License-Identifier: AGPL-3.0-or-later
app: `sonogram` C++17 Programming Language Transpiler

[CONTRIBUTOR COPYRIGHT NOTICE]
```

## Licensor
> project owner

source code files should end with comment about `license` name, `warranty` details, `SPDX` identifier, `application` legend and `copyright` notice

```md
GNU Affero General Public License v3.0 or later
NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
SPDX-License-Identifier: AGPL-3.0-or-later
app: `sonogram` C++17 Programming Language Transpiler
Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
```

## Presets
> CMake building with dependencies

for building consider clone dependencies to separate directories and add `CMakeUserPresets.json` to avoiding fetching on clean rebuilds

```json
{
    "version": 2,
    "configurePresets": [{
        "name": "local-base_",
        "hidden": true,
        "cacheVariables": {
            "FETCHCONTENT_SOURCE_DIR_SON8__MAINLAND": "/path/to/source/directory",
            "FETCHCONTENT_SOURCE_DIR_SON8__C_HEADER": "/path/to/source/directory",
            "FETCHCONTENT_SOURCE_DIR_SON8__CXX_UNIT": "/path/to/source/directory",
            "FETCHCONTENT_SOURCE_DIR_SON8__CORE_LIB": "/path/to/source/directory"
        }
    }, {
        "name": "local-debug",
        "inherits": [ "local-base_", "debug" ],
        "displayName": "Local Debug",
        "binaryDir": "${sourceDir}/temp/presets_/local-debug"
    }, {
        "name": "local-flash",
        "inherits": [ "local-base_", "flash" ],
        "displayName": "Local Flash",
        "binaryDir": "${sourceDir}/temp/presets_/local-flash"
    }],
    "buildPresets": [
        { "name":"local-debug", "configurePreset": "local-debug" },
        { "name":"local-flash", "configurePreset": "local-flash" }
    ]
}
```
