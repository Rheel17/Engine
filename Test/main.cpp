/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */

#include <gtest/gtest.h>
#include <windows.h>

int main(int argc, char *argv[]) {
	AddDllDirectory(L"D:\\Eclipse Projects\\Engine\\Build\\Development");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

