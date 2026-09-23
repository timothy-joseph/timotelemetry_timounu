function toggle_plot(div_id)
{
	var x = document.getElementById(div_id);
	if (x.style.display === "none") {
		x.style.display = "block";
	} else {
		x.style.display = "none";
	}
}

function save_to_file()
{
	var current_date = new Date();
	var filename = current_date.getFullYear() + "_" +
		       ("0" + (current_date.getMonth() + 1)).slice(-2) + "_" +
		       ("0" + current_date.getDate()).slice(-2) + "_" +
		       ("0" + current_date.getHours()).slice(-2) +
		       ("0" + current_date.getMinutes()).slice(-2) +
		       ("0" + current_date.getSeconds()).slice(-2) +
		       ".json";
	console.log(filename);
	var a = document.createElement("a");
	var file = new Blob([JSON.stringify(all_data)], {type: 'text/plain'});
	a.href = URL.createObjectURL(file);
	a.download = filename;
	a.click();
}

function load_from_file()
{
	var uploaded_file = document.getElementById("id_file_input").files[0];
	uploaded_file.text().then(function (file_text)
	{
		live_data = 0;

		console.log(file_text);

		all_data = JSON.parse(file_text);
		console.log(all_data);

		destroy_charts();
		setup_charts();

		for (index in all_data)
			handle_dat_packet(JSON.stringify(all_data[index]));
	}
	);
}

