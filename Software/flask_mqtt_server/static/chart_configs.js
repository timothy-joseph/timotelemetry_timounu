/* clear configs: no data in them; a copy is created in other variables that
 * are going to be modified
 */
/* ECU data */
const config_rpm_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "RPM",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'RPM'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_curr_gear_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Current Gear",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Current Gear'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_tps_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "TPS",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'TPS'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_oil_pres_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Oil Pressure",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Oil Pressure'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_coolant_temp_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Coolant temperature",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Coolant temperature'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_fuel_pres_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Fuel pressure",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Fuel pressure'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_lambda_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Lambda",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Lambda'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_iat_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Intake air temperature",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Intake air temperature'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_egt1_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "EGT1",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'EGT1'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_egt2_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "EGT2",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'EGT2'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_egt3_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "EGT3",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'EGT3'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_egt4_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "EGT4",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'egt4'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_vehicle_speed_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "vehicle speed",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'vehicle speed'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_map_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Manifold air pressure",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Manifold air pressure'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_bps_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Brake pressure sensor",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Brake pressure sensor'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_oil_temp_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Oil Temperature",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Oil Temperature'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

/* racire */
const config_water_in_stanga_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Water in stanga",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Water in stanga'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_water_out_stanga_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Water out stanga",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Water out stanga'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_aer_in_stanga_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "aer in stanga",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'aer in stanga'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_aer_out_stanga_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "aer out stanga",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'aer out stanga'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_manometru_stanga_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "manometru stanga",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'manometru stanga'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_anemometru_stanga_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "anemometru stanga",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'anemometru stanga'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_water_in_dreapta_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Water in dreapta",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Water in dreapta'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_water_out_dreapta_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Water out dreapta",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Water out dreapta'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_aer_in_dreapta_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "aer in dreapta",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'aer in dreapta'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_aer_out_dreapta_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "aer out dreapta",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'aer out dreapta'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_manometru_dreapta_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "manometru dreapta",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'manometru dreapta'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_anemometru_dreapta_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "anemometru dreapta",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'anemometru dreapta'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

/* PMU data */
const config_lv_bat_volts_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Low Voltage Battery Voltage",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Low Voltage Battery Voltage'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_in_voltage_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Input Voltage",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Input Voltage'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_in_cur_bat_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Input Current Battery",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Input Current Battery'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_in_cur_alt_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Input Current Alternator",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Input Current Alternator'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_current_0_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output current 0",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output current 0'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_current_1_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output current 1",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output current 1'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_current_2_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output current 2",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output current 2'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_current_3_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output current 3",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output current 3'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_current_4_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output current 4",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output current 4'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_current_5_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output current 5",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output current 5'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_current_6_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output current 6",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output current 6'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_current_7_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output current 7",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output current 7'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_voltage_0_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output voltage 0",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output voltage 0'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_voltage_1_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output voltage 1",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output voltage 1'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_voltage_2_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output voltage 2",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output voltage 2'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_voltage_3_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output voltage 3",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output voltage 3'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_voltage_4_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output voltage 4",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output voltage 4'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_voltage_5_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output voltage 5",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output voltage 5'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_voltage_6_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output voltage 6",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output voltage 6'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_output_voltage_7_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Output voltage 7",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Output voltage 7'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};



/* VDU data */
const config_tyre_presLF_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Tyre Pressure Left-Front",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Tyre Pressure Left-Front'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_tyre_presRF_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Tyre Pressure Right-Front",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Tyre Pressure Right-Front'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_tyre_presLR_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Tyre Pressure Left-Rear",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Tyre Pressure Left-Rear'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_tyre_presRR_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Tyre Pressure Right-Rear",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Tyre Pressure Right-Rear'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_susLF_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Suspention Left-Front",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Suspention Left-Front'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_susRF_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Suspention Right-Front",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Suspention Right-Front'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_susLR_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Suspention Left-Rear",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Suspention Left-Rear'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_susRR_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Suspention Right-Rear",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Suspention Right-Rear'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_accel_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Acceleration",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Acceleration'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_gyro_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Gyro",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Gyro'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

/* DAT data */
const config_altitude_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "Altitude",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'Altitude'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

const config_gps_speed_clear = {
	type: 'line',
	data: {
		labels: [],
		datasets: [
			{
				label: "GPS speed (knots)",
				backgroundColor: 'rgb(255, 99, 132)',
				borderColor: 'rgb(255, 99, 132)',
				data: [],
				fill: false,
			},
		],
	},
	options: {
		responsive: true,
		title: {
			display: false,
			text: 'GPS speed (knots)'
		},
		tooltips: {
			mode: 'index',
			intersect: false,
		},
		hover: {
			mode: 'nearest',
			intersect: true
		},
		scales: {
			xAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Time'
				}
			}],
			yAxes: [{
				display: true,
				scaleLabel: {
					display: false,
					labelString: 'Value'
				}
			}]
		}
	}
};

/* remaining 3 data points aren't graphed: long, lat, SCS */
var config_rpm = JSON.parse(JSON.stringify(config_rpm_clear));
var config_curr_gear = JSON.parse(JSON.stringify(config_curr_gear_clear));
var config_tps = JSON.parse(JSON.stringify(config_tps_clear));
var config_oil_pres = JSON.parse(JSON.stringify(config_oil_pres_clear));
var config_coolant_temp = JSON.parse(JSON.stringify(config_coolant_temp_clear));
var config_fuel_pres = JSON.parse(JSON.stringify(config_fuel_pres_clear));
var config_lambda = JSON.parse(JSON.stringify(config_lambda_clear));
var config_iat = JSON.parse(JSON.stringify(config_iat_clear));
var config_egt1 = JSON.parse(JSON.stringify(config_egt1_clear));
var config_egt2 = JSON.parse(JSON.stringify(config_egt2_clear));
var config_egt3 = JSON.parse(JSON.stringify(config_egt3_clear));
var config_egt4 = JSON.parse(JSON.stringify(config_egt4_clear));
var config_vehicle_speed = JSON.parse(JSON.stringify(config_vehicle_speed_clear));
var config_map = JSON.parse(JSON.stringify(config_map_clear));
var config_bps = JSON.parse(JSON.stringify(config_bps_clear));
var config_oil_temp = JSON.parse(JSON.stringify(config_oil_temp_clear));
var config_lv_bat_volts = JSON.parse(JSON.stringify(config_lv_bat_volts_clear));
var config_in_voltage = JSON.parse(JSON.stringify(config_in_voltage_clear));
var config_in_cur_bat = JSON.parse(JSON.stringify(config_in_cur_bat_clear));
var config_in_cur_alt = JSON.parse(JSON.stringify(config_in_cur_alt_clear));
var config_output_current_0 = JSON.parse(JSON.stringify(config_output_current_0_clear));
var config_output_current_1 = JSON.parse(JSON.stringify(config_output_current_1_clear));
var config_output_current_2 = JSON.parse(JSON.stringify(config_output_current_2_clear));
var config_output_current_3 = JSON.parse(JSON.stringify(config_output_current_3_clear));
var config_output_current_4 = JSON.parse(JSON.stringify(config_output_current_4_clear));
var config_output_current_5 = JSON.parse(JSON.stringify(config_output_current_5_clear));
var config_output_current_6 = JSON.parse(JSON.stringify(config_output_current_6_clear));
var config_output_current_7 = JSON.parse(JSON.stringify(config_output_current_7_clear));
var config_output_voltage_0 = JSON.parse(JSON.stringify(config_output_voltage_0_clear));
var config_output_voltage_1 = JSON.parse(JSON.stringify(config_output_voltage_1_clear));
var config_output_voltage_2 = JSON.parse(JSON.stringify(config_output_voltage_2_clear));
var config_output_voltage_3 = JSON.parse(JSON.stringify(config_output_voltage_3_clear));
var config_output_voltage_4 = JSON.parse(JSON.stringify(config_output_voltage_4_clear));
var config_output_voltage_5 = JSON.parse(JSON.stringify(config_output_voltage_5_clear));
var config_output_voltage_6 = JSON.parse(JSON.stringify(config_output_voltage_6_clear));
var config_output_voltage_7 = JSON.parse(JSON.stringify(config_output_voltage_7_clear));
var config_tyre_presLF = JSON.parse(JSON.stringify(config_tyre_presLF_clear));
var config_tyre_presRF = JSON.parse(JSON.stringify(config_tyre_presRF_clear));
var config_tyre_presLR = JSON.parse(JSON.stringify(config_tyre_presLR_clear));
var config_tyre_presRR = JSON.parse(JSON.stringify(config_tyre_presRR_clear));
var config_susLF = JSON.parse(JSON.stringify(config_susLF_clear));
var config_susRF = JSON.parse(JSON.stringify(config_susRF_clear));
var config_susLR = JSON.parse(JSON.stringify(config_susLR_clear));
var config_susRR = JSON.parse(JSON.stringify(config_susRR_clear));
var config_accel = JSON.parse(JSON.stringify(config_accel_clear));
var config_gyro = JSON.parse(JSON.stringify(config_gyro_clear));
var config_altitude = JSON.parse(JSON.stringify(config_altitude_clear));
var config_gps_speed = JSON.parse(JSON.stringify(config_gps_speed_clear));

