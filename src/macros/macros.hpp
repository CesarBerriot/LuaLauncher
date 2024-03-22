//
// Created by César B. on 3/22/2024.
//

#pragma once

#define do_while_0(...) do { __VA_ARGS__ } while(0)

#define make_string(...) ((std::stringstream() << __VA_ARGS__).str())
