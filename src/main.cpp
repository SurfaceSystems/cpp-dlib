/*
 * @name dlib
 * @author Blas Fernández
 * @version prerelease 0.1
 * @copyright GNU GPL v3
 */

// 13-05-2026
// Oh shit, there we go again

// Include necessary functions
#include <iostream>
#include "parser.hpp"
#include "executor.hpp"

// Main method
int main(int argc, char** argv) {
	Executor executor;

	executor.execute(parse("msg \"hello\"")[0]);
	
	return 0;
}

