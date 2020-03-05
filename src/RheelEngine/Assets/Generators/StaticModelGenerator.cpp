/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "StaticModelGenerator.h"

namespace rheel {

Model StaticModelGenerator::operator()() {
	return Generate();
}

Model StaticModelGenerator::Generate() {
	// generate the model
	DoGenerate();
	Model model(std::move(vertices), std::move(indices));

	// reset the vectors
	vertices = {};
	indices = {};

	// return the model
	return std::move(model);
}

}