//
// Created by César B. on 3/22/2024.
//

#pragma once

#define do_while_false(...) do { __VA_ARGS__ } while(false)

#define make_string(...) ((std::stringstream() << __VA_ARGS__).str())
