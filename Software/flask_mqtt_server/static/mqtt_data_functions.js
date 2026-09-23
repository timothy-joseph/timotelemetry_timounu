function update_paragraph(id, val)
{
	var p = document.getElementById(id);
	p.textContent = val;
}

function plot_point(line_chart, chart_config, x, y)
{
	if (chart_config.data.labels.length === 20) {
		chart_config.data.labels.shift();
		chart_config.data.datasets[0].data.shift();
	}
	chart_config.data.labels.push(x);
	chart_config.data.datasets[0].data.push(y);
	line_chart.update();
}

function switch_dat_key(timestamp, key, value)
{
	switch (key) {
	case "timestamp":
		break;
	case "rpm":
		plot_point(line_chart_rpm, config_rpm, timestamp, value);
		break;
	case "curr_gear":
		plot_point(line_chart_curr_gear, config_curr_gear, timestamp,
			   value);
		break;
	case "tps":
		plot_point(line_chart_tps, config_tps, timestamp, value);
		break;
	case "oil_pres":
		plot_point(line_chart_oil_pres, config_oil_pres, timestamp,
			   value);
		break;
	case "coolant_temp":
		plot_point(line_chart_coolant_temp, config_coolant_temp,
			   timestamp, value);
		break;
	case "fuel_pres":
		plot_point(line_chart_fuel_pres, config_fuel_pres, timestamp,
			   value);
		break;
	case "lambda":
		plot_point(line_chart_lambda, config_lambda, timestamp, value);
		break;
	case "iat":
		plot_point(line_chart_iat, config_iat, timestamp, value);
		break;
	case "egt1":
		plot_point(line_chart_egt1, config_egt1, timestamp, value);
		break;
	case "egt2":
		plot_point(line_chart_egt2, config_egt2, timestamp, value);
		break;
	case "egt3":
		plot_point(line_chart_egt3, config_egt3, timestamp, value);
		break;
	case "egt4":
		plot_point(line_chart_egt4, config_egt4, timestamp, value);
		break;
	case "vehicle_speed":
		plot_point(line_chart_vehicle_speed, config_vehicle_speed,
			   timestamp, value);
		break;
	case "map":
		plot_point(line_chart_map, config_map, timestamp, value);
		break;
	case "bps":
		plot_point(line_chart_bps, config_bps, timestamp, value);
		break;
	case "oil_temp":
		plot_point(line_chart_oil_temp, config_oil_temp, timestamp,
			   value);
		break;
	case "water_in_stanga":
		plot_point(line_chart_water_in_stanga, config_water_in_stanga, timestamp, value);
		break;
	case "water_out_stanga":
		plot_point(line_chart_water_out_stanga, config_water_out_stanga, timestamp, value);
		break;
	case "aer_in_stanga":
		plot_point(line_chart_aer_in_stanga, config_aer_in_stanga, timestamp, value);
		break;
	case "aer_out_stanga":
		plot_point(line_chart_aer_out_stanga, config_aer_out_stanga, timestamp, value);
		break;
	case "manometru_stanga":
		plot_point(line_chart_manometru_stanga, config_manometru_stanga, timestamp, value);
		break;
	case "anemometru_stanga":
		plot_point(line_chart_anemometru_stanga, config_anemometru_stanga, timestamp, value);
		break;
	case "water_in_dreapta":
		plot_point(line_chart_water_in_dreapta, config_water_in_dreapta, timestamp, value);
		break;
	case "water_out_dreapta":
		plot_point(line_chart_water_out_dreapta, config_water_out_dreapta, timestamp, value);
		break;
	case "aer_in_dreapta":
		plot_point(line_chart_aer_in_dreapta, config_aer_in_dreapta, timestamp, value);
		break;
	case "aer_out_dreapta":
		plot_point(line_chart_aer_out_dreapta, config_aer_out_dreapta, timestamp, value);
		break;
	case "manometru_dreapta":
		plot_point(line_chart_manometru_dreapta, config_manometru_dreapta, timestamp, value);
		break;
	case "anemometru_dreapta":
		plot_point(line_chart_anemometru_dreapta, config_anemometru_dreapta, timestamp, value);
		break;
	case "lv_bat_volts":
		plot_point(line_chart_lv_bat_volts, config_lv_bat_volts,
			   timestamp, value);
		break;
	case "in_voltage":
		plot_point(line_chart_in_voltage, config_in_voltage,
			   timestamp, value);
		break;
	case "in_cur_bat":
		plot_point(line_chart_in_cur_bat, config_in_cur_bat,
			   timestamp, value);
		break;
	case "output_current_0":
		plot_point(line_chart_output_current_0, config_output_current_0,
			   timestamp, value);
		break;
	case "output_current_1":
		plot_point(line_chart_output_current_1, config_output_current_1,
			   timestamp, value);
		break;
	case "output_current_2":
		plot_point(line_chart_output_current_2, config_output_current_2,
			   timestamp, value);
		break;
	case "output_current_3":
		plot_point(line_chart_output_current_3, config_output_current_3,
			   timestamp, value);
		break;
	case "output_current_4":
		plot_point(line_chart_output_current_4, config_output_current_4,
			   timestamp, value);
		break;
	case "output_current_5":
		plot_point(line_chart_output_current_5, config_output_current_5,
			   timestamp, value);
		break;
	case "output_current_6":
		plot_point(line_chart_output_current_6, config_output_current_6,
			   timestamp, value);
		break;
	case "output_current_7":
		plot_point(line_chart_output_current_7, config_output_current_7,
			   timestamp, value);
		break;
	case "output_voltage_0":
		plot_point(line_chart_output_voltage_0, config_output_voltage_0,
			   timestamp, value);
		break;
	case "output_voltage_1":
		plot_point(line_chart_output_voltage_1, config_output_voltage_1,
			   timestamp, value);
		break;
	case "output_voltage_2":
		plot_point(line_chart_output_voltage_2, config_output_voltage_2,
			   timestamp, value);
		break;
	case "output_voltage_3":
		plot_point(line_chart_output_voltage_3, config_output_voltage_3,
			   timestamp, value);
		break;
	case "output_voltage_4":
		plot_point(line_chart_output_voltage_4, config_output_voltage_4,
			   timestamp, value);
		break;
	case "output_voltage_5":
		plot_point(line_chart_output_voltage_5, config_output_voltage_5,
			   timestamp, value);
		break;
	case "output_voltage_6":
		plot_point(line_chart_output_voltage_6, config_output_voltage_6,
			   timestamp, value);
		break;
	case "output_voltage_7":
		plot_point(line_chart_output_voltage_7, config_output_voltage_7,
			   timestamp, value);
		break;
	case "in_cur_alt":
		plot_point(line_chart_in_cur_alt, config_in_cur_alt,
			   timestamp, value);
		break;
	case "tyre_presLF":
		plot_point(line_chart_tyre_presLF, config_tyre_presLF,
			   timestamp, value);
		break;
	case "tyre_presRF":
		plot_point(line_chart_tyre_presRF, config_tyre_presRF,
			   timestamp, value);
		break;
	case "tyre_presLR":
		plot_point(line_chart_tyre_presLR, config_tyre_presLR,
			   timestamp, value);
		break;
	case "tyre_presRR":
		plot_point(line_chart_tyre_presRR, config_tyre_presRR,
			   timestamp, value);
		break;
	case "susLF":
		plot_point(line_chart_susLF, config_susLF, timestamp, value);
		break;
	case "susRF":
		plot_point(line_chart_susRF, config_susRF, timestamp, value);
		break;
	case "susLR":
		plot_point(line_chart_susLR, config_susLR, timestamp, value);
		break;
	case "susRR":
		plot_point(line_chart_susRR, config_susRR, timestamp, value);
		break;
	case "accel":
		plot_point(line_chart_accel, config_accel, timestamp, value);
		break;
	case "gyro":
		plot_point(line_chart_gyro, config_gyro, timestamp, value);
		break;
	case "long":
		update_paragraph("id_long",
				 value + " (at time " + timestamp + ")");
		break;
	case "lat":
		update_paragraph("id_lat",
				 value + " (at time " + timestamp + ")");
		break;
	case "altitude":
		plot_point(line_chart_altitude, config_altitude, timestamp,
			   value);
		break;
	case "gps_speed":
		plot_point(line_chart_gps_speed, config_gps_speed, timestamp,
			   value);
		break;
	case "scs":
		update_paragraph("id_scs",
				 value + " (at time " + timestamp + ")");
		break;
	}
}

function plot_dat_packet(dat_packet)
{
	const timestamp = dat_packet["timestamp"];

	for (var key in dat_packet)
		switch_dat_key(timestamp, key, dat_packet[key]);
}

function handle_dat_packet(dat_packet)
{
	plot_dat_packet(JSON.parse(dat_packet));
	all_data.push(JSON.parse(dat_packet));
}

