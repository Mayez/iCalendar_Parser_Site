// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
	
	$('#fileAdd').click(function(event){
		let e = document.getElementById("fileCreate");
		let fileName = e.options[e.selectedIndex].text;
		$('#texting').html(fileName);		
	});
	$('#eventAdd').click(function(event){
		let e = document.getElementById("options2");
		let createTime = "";
		let createDate = "";
		let startTime = "";
		let startDate = "";
		let fileName = e.options[e.selectedIndex].text;
		let summary = document.getElementsByName('summary')[0].value;
		let uid = document.getElementsByName('uid')[0].value;
	
		let createYear = document.getElementsByName('createYear')[0].value;
		let createMonth = document.getElementsByName('createMonth')[0].value;
		let createDay = document.getElementsByName('createDay')[0].value;

		let createHr = document.getElementsByName('createHr')[0].value;
		let createMin = document.getElementsByName('createMin')[0].value;
		let createSec = document.getElementsByName('createSec')[0].value;

		let startYear = document.getElementsByName('startYear')[0].value;
		let startMonth = document.getElementsByName('startMonth')[0].value;
		let startDay = document.getElementsByName('startDay')[0].value;

		let startHr = document.getElementsByName('startHr')[0].value;
		let startMin = document.getElementsByName('startMin')[0].value;
		let startSec = document.getElementsByName('startSec')[0].value;

		let a = document.getElementById("utc1");
		let utc1 = a.options[a.selectedIndex].value;

		let b = document.getElementById("utc2");
		let utc2 = b.options[b.selectedIndex].value;
		
/*		$('#field').html('');
		$('#field').html(fileName);
		$('#field').append("<br>\""+summary+"\"");
		$('#field').append("<br>\""+uid+"\"");
		$('#field').append("<br>UTC1:\""+utc1+"\"");
		$('#field').append("<br>UTC2:\""+utc2+"\"");
	*/	
		if(uid == "" || document.getElementById('createHr').value == "" ||
		document.getElementById('createMin').value == "" || document.getElementById('createSec').value == "" ||
		document.getElementById('startHr').value == "" || document.getElementById('startMin').value == "" ||
		document.getElementById('startSec').value == "" || document.getElementById('createYear').value == "" ||
		document.getElementById('createMonth').value == "" || document.getElementById('createDay').value == "" ||
		document.getElementById('startYear').value == "" || document.getElementById('startMonth').value == "" ||
		document.getElementById('startDay').value == "")  
		{
			$('#tbstatus').append('<tr><th>ADD EVENT ERROR</th>' +
			'<th>' + "FILL IN ALL REQUIRED FORMS" + '</th></tr>');				
		}

		if(createHr < 10 && document.getElementById('createHr').value.length < 2)
		{
			createTime += '0';
		}
		createTime += document.getElementsByName('createHr')[0].value;
		if(createMin < 10 && document.getElementById('createMin').value.length < 2)
		{
			createTime += '0';
		}
		createTime += document.getElementsByName('createMin')[0].value;
		if(createSec < 10 && document.getElementById('createSec').value.length < 2)
		{
			createTime += '0';
		}
		createTime += document.getElementsByName('createSec')[0].value;
		if(utc1 == "Yes")
		{
			createTime += 'Z';
		}


		if(createYear < 1000 && document.getElementById('createYear').value.length < 4)
		{
			createDate += '0';
		}
		else if(createYear < 100 && document.getElementById('createYear').value.length < 3)
		{
			createDate += '00';
		}
		else if(createYear < 10 && document.getElementById('createYear').value.length < 2)
		{
			createDate += '000';
		}
		else if(createYear <= 0 && document.getElementById('createYear').value.length < 1)
		{
			createDate += '0000';
		}
		createDate += document.getElementsByName('createYear')[0].value;
		if(createMonth < 10 && document.getElementById('createMonth').value.length < 2)
		{
			createDate += '0';
		}
		createDate += document.getElementsByName('createMonth')[0].value;
		if(createDay < 10 && document.getElementById('createDay').value.length < 2)
		{
			createDate += '0';
		}
		createDate += document.getElementsByName('createDay')[0].value;



		if(startHr < 10 && document.getElementById('startHr').value.length < 2)
		{
			startTime += '0';
		}
		startTime += document.getElementsByName('startHr')[0].value;
		if(startMin < 10 && document.getElementById('startMin').value.length < 2)
		{
			startTime += '0';
		}
		startTime += document.getElementsByName('startMin')[0].value;
		if(startSec < 10 && document.getElementById('startSec').value.length < 2)
		{
			startTime += '0';
		}
		startTime += document.getElementsByName('startSec')[0].value;
		if(utc2 == "Yes")
		{
			startTime += 'Z';
		}
		
		if(startYear < 1000 && document.getElementById('startYear').value.length < 4)
		{
			startDate += '0';
		}
		else if(startYear < 100 && document.getElementById('startYear').value.length < 3)
		{
			startDate += '00';
		}
		else if(startYear < 10 && document.getElementById('startYear').value.length < 2)
		{
			startDate += '000';
		}
		else if(startYear <= 0 && document.getElementById('startYear').value.length < 1)
		{
			startDate += '0000';
		}
		startDate += document.getElementsByName('startYear')[0].value;
		if(startMonth < 10 && document.getElementById('startMonth').value.length < 2)
		{
			startDate += '0';
		}
		startDate += document.getElementsByName('startMonth')[0].value;
		if(startDay < 10 && document.getElementById('startDay').value.length < 2)
		{
			startDate += '0';
		}
		startDate += document.getElementsByName('startDay')[0].value;

	
/*	if(/^\d+$/.test(document.getElementsByName('createHr')[0].value) == true)
	{
//		$('#field').append("<br>CREATETIME:\"" + createTime +"\"");		
		$('#field').append("<br>CREATEHR:\"deded\"");		

	}
	else
	{
		$('#field').append("<br>CREATETIME FAILED 2");					
	}
*/
		if(/^\d+$/.test(document.getElementsByName('createHr')[0].value) == true &&
		/^\d+$/.test(document.getElementsByName('createMin')[0].value) == true &&
		/^\d+$/.test(document.getElementsByName('createSec')[0].value) == true)
		{
			if(document.getElementsByName('createHr')[0].value <= 23 && 
			document.getElementsByName('createHr')[0].value >= 0 &&
			document.getElementsByName('createMin')[0].value <= 59 &&
			document.getElementsByName('createMin')[0].value >= 0 &&
			document.getElementsByName('createSec')[0].value <= 99 &&
			document.getElementsByName('createSec')[0].value >= 0)
			{
		//		$('#field').append("<br>CREATETIME:\"" + createTime +"\"");

			}
			else
			{
				$('#tbstatus').append('<tr><th>ADD EVENT FIELD NUMBER ERROR</th>' +
				'<th>' + "ONE OF THE CREATE TIME FIELDS HAS AN INVALID NUMBER" + '</th></tr>');	
			}
		}
		else
		{
			$('#tbstatus').append('<tr><th>ADD EVENT FIELD ERROR</th>' +
			'<th>' + "ONE OF THE CREATE TIME FIELDS IS NOT A NUMBER" + '</th></tr>');	
		}		

		if(/^\d+$/.test(createYear) == true &&
		/^\d+$/.test(createMonth) == true &&
		/^\d+$/.test(createDay) == true)
		{
			if(createYear >= 1 && (createMonth <= 12 && createMonth >= 1) &&
			(createDay <= 31 && createDay >= 1))
			{
				if((createMonth == 1 || createMonth == 3 || createMonth == 5 ||
				createMonth == 7 || createMonth == 8 || createMonth == 10 || 
				createMonth == 12) && (createDay <= 31 && createDay >= 1))
				{
	//				$('#field').append("<br>CREATEDATE10:\"" + createDate +"\"");				
				}
				else if((createMonth == 4 || createMonth == 6 || createMonth == 9 ||
				createMonth == 11) && (createDay <= 30 && createDay >= 1))
				{
		//			$('#field').append("<br>CREATEDATE11:\"" + createDate +"\"");				
				}
				else if((createMonth == 2) && (createDay <= 28 && createDay >= 1))
				{
	//				$('#field').append("<br>CREATEDATE12:\"" + createDate +"\"");				
				}

				else if((createDay <= 29 && createDay >= 1) && 
				((createYear % 4 == 0) && (createYear % 100 != 0)) || (createYear % 400 == 0))
				{
	//				$('#field').append("<br>CREATEDATE13:\"" + createDate +"\"");				
				}

				else
				{
					$('#tbstatus').append('<tr><th>ADD EVENT FIELD NUMBER ERROR</th>' +
					'<th>' + "ONE OF THE CREATE DATE FIELDS HAS AN INVALID NUMBER" + '</th></tr>');	
				}			
			}
			else
			{
				$('#tbstatus').append('<tr><th>ADD EVENT FIELD NUMBER ERROR</th>' +
				'<th>' + "ONE OF THE CREATE DATE FIELDS HAS AN INVALID NUMBER" + '</th></tr>');	
			}			

		}
		else
		{
			$('#tbstatus').append('<tr><th>ADD EVENT FIELD ERROR</th>' +
			'<th>' + "ONE OF THE CREATE DATE FIELDS IS NOT A NUMBER" + '</th></tr>');	
		}			


		if(/^\d+$/.test(document.getElementsByName('startHr')[0].value) == true &&
		/^\d+$/.test(document.getElementsByName('startMin')[0].value) == true &&
		/^\d+$/.test(document.getElementsByName('startSec')[0].value) == true)
		{
			if(document.getElementsByName('startHr')[0].value <= 23 && 
			document.getElementsByName('startHr')[0].value >= 0 && 
			document.getElementsByName('startMin')[0].value <= 59 &&
			document.getElementsByName('startMin')[0].value >= 0 &&
			document.getElementsByName('startSec')[0].value <= 99 &&
			document.getElementsByName('startSec')[0].value >= 0)
			{
	//			$('#field').append("<br>STARTTIME:\"" + startTime +"\"");
			}
			else
			{
				$('#tbstatus').append('<tr><th>ADD EVENT FIELD NUMBER ERROR</th>' +
				'<th>' + "ONE OF THE START TIME FIELDS HAS AN INVALID NUMBER" + '</th></tr>');	
			}			

		}
		else
		{
			$('#tbstatus').append('<tr><th>ADD EVENT FIELD ERROR</th>' +
			'<th>' + "ONE OF THE START TIME FIELDS IS NOT A NUMBER" + '</th></tr>');	
		}			

		if(/^\d+$/.test(startYear) == true &&
		/^\d+$/.test(startMonth) == true &&
		/^\d+$/.test(startDay) == true)
		{
			if(startYear >= 1 && (startMonth <= 12 && startMonth >= 1) &&
			(startDay <= 31 && startDay >= 1))
			{
				if((startMonth == 1 || startMonth == 3 || startMonth == 5 ||
				startMonth == 7 || startMonth == 8 || startMonth == 10 || 
				startMonth == 12) && (startDay <= 31 && startDay >= 1))
				{
		//			$('#field').append("<br>STARTDATE10:\"" + startDate +"\"");				
				}
				else if((startMonth == 4 || startMonth == 6 || startMonth == 9 ||
				startMonth == 11) && (startDay <= 30 && startDay >= 1))
				{
		//			$('#field').append("<br>STARTDATE11:\"" + startDate +"\"");				
				}
				else if((startMonth == 2) && (startDay <= 28 && startDay >= 1))
				{
		//			$('#field').append("<br>STARTDATE12:\"" + startDate +"\"");				
				}

				else if((startDay <= 29 && startDay >= 1) && 
				((startYear % 4 == 0) && (startYear % 100 != 0)) || (startYear % 400 == 0))
				{
		//			$('#field').append("<br>STARTDATE13:\"" + startDate +"\"");				
				}

				else
				{
					$('#tbstatus').append('<tr><th>ADD EVENT FIELD NUMBER ERROR</th>' +
					'<th>' + "ONE OF THE START DATE FIELDS HAS AN INVALID NUMBER" + '</th></tr>');	
				}			
			}
			else
			{
				$('#tbstatus').append('<tr><th>ADD EVENT FIELD NUMBER ERROR</th>' +
				'<th>' + "ONE OF THE START DATE FIELDS HAS AN INVALID NUMBER" + '</th></tr>');	
			}			

		}
		else
		{
			$('#tbstatus').append('<tr><th>ADD EVENT FIELD ERROR</th>' +
			'<th>' + "ONE OF THE START DATE FIELDS IS NOT A NUMBER" + '</th></tr>');	
		}

/*		//createTime
*/

/*		$.get("/calendar", {field1: fileName, field2: },function(data){
			
		}):*/
	});
	
	$('#dbStatus').click(function(event){
		$.get("/dbCount",function(data){		
			if(data[0] != undefined && data[1] != undefined && data[2] != undefined)
			{ 
				$('#tbstatus').append('<tr><th>DATABASE STATUS</th>' +
				'<th>' + 'Database has '+ data[0] +' files, ' +data[1] +' events, and ' + data[2] +' alarms' + '</th></tr>');			
			}
		});
	});
	
	$('#dblogin').click(function(event){
		let username = $('#username').val();
		let password = $('#password').val();
		let dbname = $('#dbname').val();
/*		$('#testing1').html("WORKS");
		$('#testing1').append(username+'<br>'+password+'<br>'+dbname);*/
		$.get("/dbConnect",{username:username, password:password, dbname:dbname},function(data){
			if(data.code == 1)
			{
				$('#testing1').html("COULD NOT CONNECT TO DB");
				$('#tbstatus').append('<tr><th>DATABASE STATUS</th>' +
				'<th>' + 'COULD NOT CONNECT TO DATABASE' + '</th></tr>');
			}
			else if(data.code == 0)
			{
				$('#testing1').html("CONNECTED TO DB");				
				$('#tbstatus').append('<tr><th>DATABASE STATUS</th>' +
				'<th>' + 'CONNECTED TO DATABASE' + '</th></tr>');
			}
		});
	});

	$('#clearDb').click(function(event){
		$.get("/clearDb",function(data){
			if(data.code == 1 || data.code == 2 || data.code == 3)
			{
				$('#testing2').html(data.msg);
			}
			else
			{
				$('#testing2').html("ALL TABLES DELETED");				
			}
			
		});
	});


	$('#displayEvt').click(function(event){
		$.get("/displayEvt",function(data){
//			$('#testing3').html(JSON.stringify(data));
			$('#sortDate').html('');
			for(let k = 0; k < data.length; k++)
			{
				let dateVal = "";
				let timeVal = "";
				for(let j = 0; j < data[k].start_time.length; j++)
				{
					if(j < 10)
						dateVal += data[k].start_time[j];
					else if(j > 10 && j < 19) 
						timeVal += data[k].start_time[j];
				}
				let summary = ' ';
				let organizer = ' ';
				let location = ' ';
				if(data[k].summary != null)
					summary = data[k].summary;
				if(data[k].organizer != null)
					organizer = data[k].organizer;
				if(data[k].location != null)
					location = data[k].location;
				$('#sortDate').append('<tr>');
				$('#sortDate').append('<th>' + data[k].event_id + '</th>' +
					'<th>' + summary + '</th>'+
					'<th>' + "Date: " + dateVal + ", Time: " + timeVal + '</th>' +
					'<th>' + location + '</th>' +
					'<th>' + organizer + '</th></tr>');
			}
		});
	});
	
	$('#storeFiles').click(function(event){
		$.get("/storeFiles",function(data){
		});
	});
	
	$('#conflictEvt').click(function(event){
		
		$.get("/conflictEvt",function(data){
			$('#sortDate').html('');
			for(let k = 0; k < data.length; k++)
			{
/*				let dateVal = "";
				let timeVal = "";
				for(let j = 0; j < data[k].start_time.length; j++)
				{
					if(j < 10)
						dateVal += data[k].start_time[j];
					else if(j > 10 && j < 19) 
						timeVal += data[k].start_time[j];
				}*/
				let summary = ' ';
				let organizer = ' ';
				let location = ' ';
				if(data[k].summary != null)
					summary = data[k].summary;
				if(data[k].organizer != null)
					organizer = data[k].organizer;
				if(data[k].location != null)
					location = data[k].location;
				$('#sortDate').append('<tr>');
				$('#sortDate').append('<th>' + data[k].event_id + '</th>' +
					'<th>' + summary + '</th>'+
					'<th>' + data[k].start_time + '</th>' +
					'<th>' + location + '</th>' +
					'<th>' + organizer + '</th></tr>');
			}							
		});
	});

	$('#fileSel').change(function(event){
		let e = document.getElementById("fileSel");
		let fileName = e.options[e.selectedIndex].text;
//		$('#testing3').html("CAL: " + fileName);		
		$('#evtByFile').click(function(event)
		{
			$.get("/fileEvt", {field1: fileName},function(data){
				if(data != undefined)
				{
					$('#sortDate').html('');
					for(let k = 0; k < data.length; k++)
					{
						let dateVal = "";
						let timeVal = "";
						for(let j = 0; j < data[k].start_time.length; j++)
						{
							if(j < 10)
								dateVal += data[k].start_time[j];
							else if(j > 10 && j < 19) 
								timeVal += data[k].start_time[j];
						}
						let summary = ' ';
						let organizer = ' ';
						let location = ' ';
						if(data[k].summary != null)
							summary = data[k].summary;
						if(data[k].organizer != null)
							organizer = data[k].organizer;
						if(data[k].location != null)
							location = data[k].location;
						$('#sortDate').append('<tr>');
						$('#sortDate').append('<th>' + data[k].event_id + '</th>' +
							'<th>' + summary + '</th>'+
							'<th>' + "Date: " + dateVal + ", Time: " + timeVal + '</th>' +
							'<th>' + location + '</th>' +
							'<th>' + organizer + '</th></tr>');
					}				
				}
			});
		});
	});

	
	$('#options').change(function(event){
		let e = document.getElementById("options");
		let fileName = e.options[e.selectedIndex].text;
//		$('#output').html("CAL: " + fileName);
		$.get("/calendar", {field1: fileName},function(data){
//			$('#jsonoutput').html("CAL: " + JSON.stringify(data));
			$('#tbheader').html('<th>Event No</th>' +
			'<th>Start date</th>' +
			'<th>Start time</th>' +
			'<th>Summary</th>' +
			'<th>Props</th> '+
			'<th>Alarms</th>');
			$('#tbcalendar').html('');			
			for(let i = 0; i < data.length; i++)
			{
				let dateVal = "";
				for(let k = 0; k < data[i].startDT.date.length; k++)
				{
					if(k == 4 || k == 6)
						dateVal += '/';
					dateVal += data[i].startDT.date[k];
				}
				
				let timeVal = "";
				for(let m = 0; m < data[i].startDT.time.length; m++)
				{
					if(m == 2 || m == 4)
						timeVal += ':';
					timeVal += data[i].startDT.time[m];
				}
				$('#tbcalendar').append('<tr>');
				$('#tbcalendar').append('<th>' + (i+1) + '</th>' +
				'<th>' + dateVal + '</th>');
				if(data[i].startDT.isUTC == true)
				{
					$('#tbcalendar').append('<th>' +
					timeVal + '(UTC)' + '</th>');
				}
				else
				{
					$('#tbcalendar').append('<th>' +
					timeVal + '</th>');					
				}
				$('#tbcalendar').append('<th>' +
				data[i].summary + '</th>' +
				'<th>' + data[i].numProps + '</th>' +
				'<th>' + data[i].numAlarms + '</th></tr>'); 
			}
			
			$('#options1').html("<option> </option>");			
			for(let i = 0; i < data.length; i++)
			{
				$('#options1').append("<option>" + (i+1) + "</option>");				
				
			}

		});
		
	});


	//might not be working because of the .change for (#options)
	$('#displayAlarms').click(function(event){
		let e = document.getElementById("options");
		let fileName = e.options[e.selectedIndex].text;
		if(fileName != "" && fileName != undefined)
		{
	//		$('#output').html("CAL1: " +fileName);
			$.get("/calendar1", {field1: fileName},function(data){
		//		$('#jsonoutput').html("CAL1: " + JSON.stringify(data));
				let e = document.getElementById("options1");
				let eventNo = e.options[e.selectedIndex].text;
				let indexVal = Number(eventNo);
				if(eventNo != "" && eventNo != undefined)	
				{
					$('#tbheader').html('');
					$('#tbcalendar').html('');

					$('#tbheader').append('<th>Action</th>' +
					'<th>Trigger</th>' +
					'<th>Props</th>');
					
					$('#output').html('');
/*					$('#output').append("EVENT: \"" + indexVal + "\"");				
					$('#output').append("NumProps: " + data[indexVal-1].numProps);
					$('#output').append("AlarmLength: " + data[indexVal-1].alarmList.length);
*/					if(data[indexVal-1].alarmList.length > 0)
					{
						for(let k = 0; k < data[indexVal-1].alarmList.length; k++)
						{
							let item = data[indexVal-1].alarmList[k];
							$('#tbcalendar').append('<tr>');
							$('#tbcalendar').append('<th>' + fileName + '/' +
							data[indexVal-1].alarmList[k].action + '</th>');
							if(item.trigger.split('VALUE=DATE-TIME:')[1] != undefined)
							{
								let item1 = item.trigger.split('VALUE=DATE-TIME:')[1];							
								let date = item1.split('T')[0];
								let dateVal = "";
								for(let k = 0; k < date.length; k++)
								{
									if(k == 4 || k == 6)
										dateVal += '/';
									dateVal += date[k];
								}

								let time = item1.split('T')[1];
								let timeVal = "";
								for(let m = 0; m < time.length; m++)
								{
									if(m == 2 || m == 4)
										timeVal += ':';
									if(time[m] != 'Z')
									{
										timeVal += time[m];
									}
									if(time[m] == 'Z')
									{
										timeVal += '(UTC)';
									}
								}
								$('#tbcalendar').append('<th> Date:' + dateVal + ", Time:" + timeVal + '</th>');								
								$('#tbcalendar').append('</th>');
							}
							else
							{
								$('#tbcalendar').append('<th>' + data[indexVal-1].alarmList[k].trigger+ '</th>');
							}
							$('#tbcalendar').append('<th>' + data[indexVal-1].alarmList[k].numProps + '</th></tr>');
						}
					}
					else
					{
							$('#tbcalendar').append('<tr><th colspan = "3">No Alarms</th></tr>');						
					}
					
				}

			});
		}
	});

	$('#displayOptProps').click(function(event){
		let e = document.getElementById("options");
		let fileName = e.options[e.selectedIndex].text;
		if(fileName != "" && fileName != undefined)
		{
	//		$('#output').html("CAL2: " +fileName);
			$.get("/calendar2", {field1: fileName},function(data){
		//		$('#jsonoutput').html("CAL2: " + JSON.stringify(data));
				let e = document.getElementById("options1");
				let eventNo = e.options[e.selectedIndex].text;
				let indexVal = Number(eventNo);
				if(eventNo != "" && eventNo != undefined)	
				{
					$('#tbheader').html('');
					$('#tbcalendar').html('');

					$('#tbheader').append('<th>Property Name</th>' +
					'<th>Property Description</th>');
					
					$('#output').html('');
					$('#output').append("EVENT: \"" + indexVal + "\"");				
					$('#output').append("NumProps: " + data[indexVal-1].numProps);
					$('#output').append("AlarmLength: " + data[indexVal-1].alarmList.length);
					if(data[indexVal-1].propertyList.length > 0)
					{
						for(let k = 0; k < data[indexVal-1].propertyList.length; k++)
						{
							let item = data[indexVal-1].propertyList[k];
							if(item.propName != "SUMMARY")
							{
								if(item.propName == "LAST-MODIFIED" || item.propName == "CREATED" ||
								item.propName == "DTEND" )
								{	
									$('#tbcalendar').append('<tr>');
									$('#tbcalendar').append('<th>' + fileName + '/' + 
									item.propName + '</th>');
									let date = item.propDescr.split('T')[0];
									let dateVal = "";
									for(let k = 0; k < date.length; k++)
									{
										if(k == 4 || k == 6)
											dateVal += '/';
										dateVal += date[k];
									}

									let time = item.propDescr.split('T')[1];
									let timeVal = "";
									for(let m = 0; m < time.length; m++)
									{
										if(m == 2 || m == 4)
											timeVal += ':';
										if(time[m] != 'Z')
										{
											timeVal += time[m];
										}
										if(time[m] == 'Z')
										{
											timeVal += '(UTC)';
										}
									}
									$('#tbcalendar').append('<th> Date:' + dateVal + ", Time:" + timeVal + '</th>');								
									$('#tbcalendar').append('</th>');
								}
								else if(item.propName == "ORGANIZER")
								{
									$('#tbcalendar').append('<tr>');
									$('#tbcalendar').append('<th>' + fileName + '/' + 
									item.propName + '</th>');
									let organizer = item.propDescr.split('mailto:').pop();
									$('#tbcalendar').append('<th>' + organizer + '</th>');								
									$('#tbcalendar').append('</th>');
								}
								else
								{
									$('#tbcalendar').append('<tr><th>' + fileName + '/' +
									item.propName + '</th>' +
									'<th>' + item.propDescr + '</th></tr>');
								}
							}
						}
					}
					else
					{
							$('#tbcalendar').append('<tr><th colspan = "2">No Other Properties</th></tr>');						
					}

				}

			});
		}
	});

	$('#clear').click(function(){
		$('#tbstatus').empty();
	});

    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/filepanel',   //The server endpoint we are connecting to
        success: function (data) {			
			if(data.length > 0)
			{
				for( y in data)
				{
					if(data[y].filename.split('.').pop() == "ics")
					{
						if(data[y].version != 0)
						{
							$('#tbfile').append('<tr><th><a href = "/uploads/' + data[y].filename + '"><u>' +
							data[y].filename + '</u></a></th>' + 
							'<th>' + data[y].version.toFixed(1) + '</th>' +
							'<th>' + data[y].prodID + '</th>' +
							'<th>' + data[y].numEvents + '</th>' +
							'<th>' + data[y].numProps + '</th></tr>');
						}
						else
						{
							$('#tbfile').append('<tr><th><a href = "/uploads/' + data[y].filename + '"><u>' +
							data[y].filename + '</u></a></th>' + 
							'<th> </th>' +
							'<th>' + data[y].prodID + '</th>' +
							'<th> </th>' +
							'<th> </th></tr>');
						}
					}
				}
			}
			else
			{
				$('#tbfile').html('<tr><th colspan = "5">No files</th></tr>');
			}
			
			$('#options').html("<option> </option>");
			$('#options2').html("<option> </option>");
			$('#fileSel').html("<option> </option>");
    		for(let x = 0; x < data.length; x++)
			{
				if(data[x].filename.split('.').pop() == "ics")
				{
					$('#options').append("<option>" + data[x].filename + "</option>");				
					$('#options2').append("<option>" + data[x].filename + "</option>");				
					if(data[x].prodID == "invalid file")
					{
						$('#tbstatus').append('<tr><th>' + data[x].filename + ' file violation error</th>' +
						'<th>This file does not meet the RFC 5545 icalendar requirements</th></tr>');					
					}
					else
					{
						$('#fileSel').append("<option>" + data[x].filename + "</option>");										
						$('#fileSel1').append("<option>" + data[x].filename + "</option>");										

					}
				}
			}
			
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log("THERE WAS AN ERROR WITH DISPLAYING THE FILES IN THE FILE PANEL"); 
        }
    });

    // Event listener form replacement example, building a Single-Page-App, no redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Callback from the form");
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the 
        $.ajax({});
    });
    
});
