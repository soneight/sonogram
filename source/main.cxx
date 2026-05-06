#include <app.hxx>
// son8
#include <son8/main.hxx>
// std
#include <iostream>

APP_PROC son8::main( Args args ) try {
   std::cout << "sonogram" << std::endl;
} catch ( ... ) {
   std::cerr << "... uncaught exception" << std::endl;
}

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
