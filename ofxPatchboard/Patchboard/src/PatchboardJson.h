#pragma once

#include <string>
#include "json/json.h"
#include "PatchboardBase.h"

//!
//! Patchboard derivative class for parsing JSON into a patchboard
//! with a patchcords map.
//!
//! Example:
//! {
//!		"patchboard":{
//!			"settings":{
//!				"input":{
//!					"type":"EmotiBit"
//!				},
//! 			"output" : {
//! 				"type":"LSL",
//! 				"meta-data" : {
//! 					"channels":[
//! 						{"name":"PPG_RED", "type" : "PPGRed", "nominal_srate" : 25.0},
//! 						{ "name":"PPG_IR", "type" : "PPGInfrared", "nominal_srate" : 25.0 },
//!							{ "name":"PPG_GRN", "type" : "PPGGreen", "nominal_srate" : 25.0 },
//!							{ "name":"HR", "type" : "HeartRate", "nominal_srate" : 0.0 },
//! 						{ "name":"EDA", "type" : "EDA", "nominal_srate" : 15.0 }
//! 					]
//! 				}
//! 			}
//! 		},
//! 		"patchcords":[
//! 			{"input":"PR", "output" : "PPG_RED"},
//! 			{ "input":"PI", "output" : "PPG_IR" },
//! 			{ "input":"PG", "output" : "PPG_GRN" },
//! 			{ "input":"HR", "output" : "HR" },
//! 			{ "input":"EA", "output" : "EDA" },
//!			]
//! 	}
//! }
//!
class PatchboardJson : public PatchboardBase
{
public:

	Json::Value patchboard;

	//! Parses a string in JSON format to create a patchboard
	//! @param patchboardStr JSON formatted string to parse
	//! @return PatchboardBase::ReturnCode SUCCESS==0
	ReturnCode parse(const std::string &patchboardStr);

	PatchboardJson& operator=(const PatchboardJson & p);

};