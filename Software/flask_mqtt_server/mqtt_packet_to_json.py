import json

# binary mqtt packet:
# 4 bytes containing the timestamp
# 2 bytes containing the CAN id
# 1 byte contatining the data_size
# then data_size bytes representing the data
class decoded_mqtt_packet:
    def __init__(self, hex_string):
        bin_data = bytes.fromhex(hex_string);

        self.vehicle_status = dict();
        self.vehicle_status["timestamp"] = (bin_data[0] << 24) | (bin_data[1] << 16) | \
                         (bin_data[2] << 8) | bin_data[3];

        self.vehicle_status["rpm"] = bin_data[4] * 100;
        self.vehicle_status["curr_gear"] = bin_data[5];
        self.vehicle_status["tps"] = bin_data[6];
        self.vehicle_status["oil_pres"] =  bin_data[7];
        self.vehicle_status["coolant_temp"] = bin_data[8];
        self.vehicle_status["fuel_pres"] = bin_data[9] * (2 ** (-3));
        self.vehicle_status["lambda"] = bin_data[10] * (2 ** (-7));
        self.vehicle_status["iat"] = bin_data[11] * (2 ** (-3));
        self.vehicle_status["egt1"] = bin_data[12] * 100;
        self.vehicle_status["egt2"] = bin_data[13] * 100;
        self.vehicle_status["egt3"] = bin_data[14] * 100;
        self.vehicle_status["egt4"] = bin_data[15] * 100;
        self.vehicle_status["vehicle_speed"] = bin_data[16];
        self.vehicle_status["map"] = bin_data[17];
        self.vehicle_status["bps"] = bin_data[18];
        self.vehicle_status["oil_temp"] = bin_data[19];
        self.vehicle_status["susLF"] = bin_data[20];
        self.vehicle_status["susRF"] = bin_data[21];
        self.vehicle_status["susLR"] = bin_data[22];
        self.vehicle_status["susRR"] = bin_data[23];
        self.vehicle_status["accel"] = (bin_data[24] << 8) | bin_data[25];
        self.vehicle_status["gyro"] = (bin_data[26] << 8) | bin_data[27];
        self.vehicle_status["tyre_presLF"] = bin_data[28] * (2 ** (-3));
        self.vehicle_status["tyre_presRF"] = bin_data[29] * (2 ** (-3));
        self.vehicle_status["tyre_presLR"] = bin_data[30] * (2 ** (-3));
        self.vehicle_status["tyre_presRR"] = bin_data[31] * (2 ** (-3));
        self.vehicle_status["lv_bat_volts"] = bin_data[32]* (2 ** (-3));
        self.vehicle_status["in_voltage"] = bin_data[33]* (2 ** (-3));
        self.vehicle_status["in_cur_bat"] = bin_data[34]* (2 ** (-3));
        self.vehicle_status["in_cur_alt"] = bin_data[35]* (2 ** (-3));
        self.vehicle_status["output_current_0"] = bin_data[36];
        self.vehicle_status["output_current_1"] = bin_data[37];
        self.vehicle_status["output_current_2"] = bin_data[38];
        self.vehicle_status["output_current_3"] = bin_data[39];
        self.vehicle_status["output_current_4"] = bin_data[40];
        self.vehicle_status["output_current_5"] = bin_data[41];
        self.vehicle_status["output_current_6"] = bin_data[42];
        self.vehicle_status["output_current_7"] = bin_data[43];
        self.vehicle_status["output_voltage_0"] = bin_data[44];
        self.vehicle_status["output_voltage_1"] = bin_data[45];
        self.vehicle_status["output_voltage_2"] = bin_data[46];
        self.vehicle_status["output_voltage_3"] = bin_data[47];
        self.vehicle_status["output_voltage_4"] = bin_data[48];
        self.vehicle_status["output_voltage_5"] = bin_data[49];
        self.vehicle_status["output_voltage_6"] = bin_data[50];
        self.vehicle_status["output_voltage_7"] = bin_data[51];
        self.vehicle_status["dash_selector"] = bin_data[52];
        self.vehicle_status["scs"] = bin_data[53];

        # racire
        self.vehicle_status["water_in_stanga"] = self.float_helper(bin_data, 54);
        self.vehicle_status["water_out_stanga"] = self.float_helper(bin_data, 58);
        self.vehicle_status["aer_in_stanga"] = self.float_helper(bin_data, 62);
        self.vehicle_status["aer_out_stanga"] = self.float_helper(bin_data, 66);
        self.vehicle_status["manometru_stanga"] = self.float_helper(bin_data, 70);
        self.vehicle_status["anemometru_stanga"] = self.float_helper(bin_data, 74);
        self.vehicle_status["water_in_dreapta"] = self.float_helper(bin_data, 78);
        self.vehicle_status["water_out_dreapta"] = self.float_helper(bin_data, 82);
        self.vehicle_status["aer_in_dreapta"] = self.float_helper(bin_data, 86);
        self.vehicle_status["aer_out_dreapta"] = self.float_helper(bin_data, 90);
        self.vehicle_status["manometru_dreapta"] = self.float_helper(bin_data, 94);
        self.vehicle_status["anemometru_dreapta"] = self.float_helper(bin_data, 98);



    def to_json(self):
        return json.dumps(self.vehicle_status);

    def float_helper(self, bin_data, start_index):
        int_value = (bin_data[start_index] << 24) | \
                    (bin_data[start_index + 1] << 16) | \
                    (bin_data[start_index + 2] << 8) | \
                    (bin_data[start_index + 3]);
        return struct.unpack('!f', struct.pack('!I', int_value))[0];

