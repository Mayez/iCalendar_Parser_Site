'use strict'

// C library API
const ffi = require('ffi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    console.log(err);
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

let calendar = ffi.Library('./libcal',{
	appFilePanel: ['string',['string']],
	appCalendarPanel: ['string',['string']]
});

//Sample endpoint
app.get('/someendpoint', function(req , res){
  res.send({
   dark : "1234",
   test: "HELLO"
  });
});

app.get('/testing', function(req , res){
  res.send({
   yoyo : "hi",
   zebra: "HELL"
  });

});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);

/*
app.post("/status",function(req,res){
	let files = fs.readdirSync('./uploads/');	
	console.log(files);
	if(req.files)
	{
		let file = req.files.filename,
		filename = file.name;
		console.log("file selected: " +filename);
		try{
			let fileUploadStatus = new Array();
			fileUploadStatus.push(filename);
			if(fs.existsSync(__dirname+'/uploads/'+filename))
			{
				let descr = filename + " already exists in the uploads directory. As a result the file will not be added";
				fileUploadStatus.push(descr);
				let code = "DUP";
				fileUploadStatus.push(code);			
				res.send(fileUploadStatus);
			}
			else
			{
				file.mv("./uploads/"+filename,function(err){
					if(err){
						console.log(err);				
					}
//					res.redirect('/');
				});
				let descr = filename + " uploading process is OK. It is now added to the uploads directory";				
				fileUploadStatus.push(descr);
				let code = "OK";
				fileUploadStatus.push(code);			
				res.send(fileUploadStatus);
//				res.redirect('back');
			}
		}catch(e){
			let fileUploadStatus = new Array();
			fileUploadStatus.push(filename);			
			let descr = filename + "\'s file upload failed. Maybe a valid file was not selected. Try again";				
			fileUploadStatus.push(descr);
			let code = "ERROR";
			fileUploadStatus.push(code);			
			res.send(fileUploadStatus);
		}
/*			let dupFile = fs.readdirSync('./uploads/'+filename);
			let length = dupFile.length;
			console.log("DUPFILELENGTH: " + length);				*/
/*	}
});*/
//uploading files to /uploads
app.post("/",function(req,res){
	if(req.files)
	{
		let length = req.files.filename.length;
//		console.log("file count: "  + length);
				
		let file = req.files.filename,
		filename = file.name;
		file.mv("./uploads/"+filename,function(err){
			if(err){
				console.log(err);				
			}
		});
	}
	res.redirect('back');
});


app.get('/filepanel', function(req,res) {
	let files = fs.readdirSync('./uploads/');
	console.log(files);	
	let fileJSON = new Array();
	for(let i = 0; i < files.length; i++){
		let fileInfo = JSON.parse(calendar.appFilePanel("./uploads/" + files[i]));
		fileInfo.filename = files[i];
		fileJSON.push(fileInfo);
	}
	console.log(fileJSON);
	res.send(fileJSON);
});

app.get('/calendar',function(req,res) {
	console.log("CALENDAR");
	let inputContent = req.query.field1;
	console.log(inputContent);
	let cps = JSON.parse(calendar.appCalendarPanel("./uploads/" + inputContent));
	console.log("LENGTH: " + cps.length);
	if(cps.length > 0)
	{
		console.log("YEAH");
//		console.log(cps);
		res.send(cps);
	}
	for(let counter = 0; counter < cps.length; counter++)
	{
		console.log("ALARMLENGTH: " +cps[counter].alarmList.length);
		for(let counter2 = 0; counter2 < cps[counter].alarmList.length; counter2++)
		{
			let action = "";
			let trigger = "";
			action = cps[counter].alarmList[counter2].action;
			trigger = cps[counter].alarmList[counter2].trigger;
			console.log("ACTION: " + cps[counter].alarmList[counter2].action);
			console.log("TRIGGER: " + cps[counter].alarmList[counter2].trigger);
/*			let alarmJSON = [[action,trigger,result[0].lastVal]];
			db.query('insert into ALARM (action, trigger, event) values (?)',alarmJSON, function(err,result){
				if(err)
				{
					console.log("ALARM ADD FAILED");
				}
			});*/
		}																															
	}
});

app.get('/calendar1',function(req,res) {
	console.log("CALENDAR1");
	let inputContent = req.query.field1;
	console.log(inputContent);
	let cps = JSON.parse(calendar.appCalendarPanel("./uploads/" + inputContent));
	console.log("LENGTH: " + cps.length);
	if(cps.length > 0)
	{
		console.log("YEAH1");
//		console.log(cps);
		res.send(cps);
	}
});

app.get('/calendar2',function(req,res) {
	console.log("CALENDAR2");
	let inputContent = req.query.field1;
	console.log(inputContent);
	let cps = JSON.parse(calendar.appCalendarPanel("./uploads/" + inputContent));
	console.log("LENGTH: " + cps.length);
	if(cps.length > 0)
	{
		console.log("YEAH2");
//		console.log(cps);
		res.send(cps);
	}
	
});

/*list of files in uploads directory
app.get('/calendardropdown',function(req, res) {
	res.send(files);
});*/

let host = "dursley.socs.uoguelph.ca";
let username = "";
let password = "";
let dbname = "";
let db = null;
const mysql = require('mysql');
let dbCorrect = 0;
let storeFlag = 0;
app.get('/dbConnect',function(req,res){
	console.log("DBCONNECT");
	username = req.query.username;
	password = req.query.password;
	dbname = req.query.dbname;
	console.log("USERNAME: "+username);
	console.log("PASSWORD: "+password);
	console.log("DBNAME: "+dbname);
	db =  mysql.createConnection({
    host     : host,
    user     : username,
    password : password,
    database : dbname
	});
	db.connect(function(err){
		if(err)
		{
			dbCorrect = 0;
			console.log("COULD NOT CONNECT TO DATABASE");
			res.send({"code": 1});
		}
		else
		{
			dbCorrect = 1;
			let fileRows = 0;
			let eventRows = 0;
			let alarmRows = 0;
			res.send({"code": 0});
			console.log("CONNECTION SUCCESSFULL");
			db.query('create table IF NOT EXISTS FILE (cal_id INT AUTO_INCREMENT PRIMARY KEY, file_name VARCHAR(60) NOT NULL, version INT NOT NULL, prod_id VARCHAR(256) NOT NULL)',function(err,result){
				if(err)
				{
					console.log("FILE TABLE FAILED TO CREATE");
				}
				else
				{
					console.log("FILE TABLE MADE");
				}
			});
			
			db.query('create table IF NOT EXISTS EVENT (event_id INT AUTO_INCREMENT PRIMARY KEY, summary VARCHAR(1024), start_time DATETIME NOT NULL, location VARCHAR(60), organizer VARCHAR(256), cal_file INT NOT NULL, CONSTRAINT FOREIGN KEY(cal_file) REFERENCES FILE(cal_id) ON DELETE CASCADE)', function(err, result){
				if(err)
				{
					console.log("EVENT TABLE FAILED TO CREATE");
				}
				else
				{
					console.log("EVENT TABLE MADE");
				}				
			});
			
			db.query('create table IF NOT EXISTS ALARM (alarm_id INT AUTO_INCREMENT PRIMARY KEY, action VARCHAR(256) NOT NULL, `trigger` VARCHAR(256) NOT NULL, event INT NOT NULL, CONSTRAINT FOREIGN KEY(event) REFERENCES EVENT(event_id) ON DELETE CASCADE)',function(err,result){
				if(err)
				{
					console.log("ALARM TABLE FAILED TO CREATE");
				}
				else
				{
					console.log("ALARM TABLE MADE");
				}							
			});
		}
	});
});

app.get('/dbCount',function(req,res){
	if(db != null && dbCorrect == 1)
	{
		db.query("SELECT COUNT(*) as fileRows FROM FILE",function(err,result){
			if(err)
			{
				console.log("TOTAL QUERY FAIL1");
			}
			else
			{
				let fileRows = result[0].fileRows;
				console.log("Total rows: " + result[0].fileRows);
				db.query("SELECT COUNT(*) as eventRows FROM EVENT",function(err,result){
					if(err)
					{
						console.log("TOTAL QUERY FAIL2");
					}
					else
					{
						let eventRows = result[0].eventRows;
						console.log("Total rows: " + result[0].eventRows);
						db.query("SELECT COUNT(*) as alarmRows FROM ALARM",function(err,result){
							if(err)
							{
								console.log("TOTAL QUERY FAIL3");
							}
							else
							{
								let alarmRows = result[0].alarmRows;
								console.log("Total rows: " + result[0].alarmRows);
								let fileJSON = new Array();									
								fileJSON.push(fileRows);
								fileJSON.push(eventRows);
								fileJSON.push(alarmRows);
								console.log("Database has "+ fileRows +" files, " +eventRows +" events, and " + alarmRows +" alarms");									
								res.send(fileJSON);
							}
						});
					}
				});
			}
		});
	}
});
app.get('/clearDb',function(req,res){
	if(db != null && dbCorrect == 1)
	{
		storeFlag = 0;
		db.query("delete from ALARM",function(err,result){
			if(err)
			{
				console.log("ALARM DELETE failed");
				res.send({"code": 3, "msg": "Alarm delete failed"});
			}
		
		});
		db.query("delete from EVENT",function(err,result){
			if(err)
			{
				console.log("EVENT DELETE failed");
				res.send({"code": 2, "msg": "Event delete failed"});
			}
		
		});
		db.query("delete from FILE",function(err,result){
			if(err)
			{
				console.log("FILE DELETE failed");
				res.send({"code": 1, "msg": "File delete failed"});
			}
			
		});
	}
});

app.get('/storeFiles',function(req,res){
	if(db != null && dbCorrect == 1)
	{
		storeFlag = 1;
		db.query('ALTER TABLE FILE AUTO_INCREMENT = 1',function(err,result){
			if(err)
			{
				console.log("AUTO INCREMENT FAILED");
			}
		});
		db.query('ALTER TABLE EVENT AUTO_INCREMENT = 1',function(err,result){
			if(err)
			{
				console.log("AUTO INCREMENT FAILED");
			}
		});
		db.query('ALTER TABLE ALARM AUTO_INCREMENT = 1',function(err,result){
			if(err)
			{
				console.log("AUTO INCREMENT FAILED");
			}
		});
		let val = 0;
		let files = fs.readdirSync('./uploads/');
		console.log("THE FILES HERE :" +files);	
		let fileJSON = new Array();
		for(let i = 0; i < files.length; i++){
			let fileInfo = JSON.parse(calendar.appFilePanel("./uploads/" + files[i]));
			fileInfo.filename = files[i];
			if(fileInfo.version != 0)
			{
				db.query('SELECT COUNT(*) as dupFile FROM FILE WHERE file_name = \"'+fileInfo.filename+'\"',function(err,result)
				{
					if(err)
					{
						console.log("The Count Failed");
					}

					else
					{
						
						if(result[0].dupFile == 0)
						{
							console.log("GOD DAMN IT: " +fileInfo.prodID);
							let fileJSON = [[fileInfo.filename, fileInfo.version, fileInfo.prodID]];
							db.query('insert into FILE (file_name, version, prod_id) values (?)',fileJSON, function(err,result){
								if(err)
								{
									console.log("FILE ADD FAILED");
								}
								else
								{
									db.query('SELECT cal_id as calFile FROM FILE WHERE file_name = \"'+fileInfo.filename+'\"',function(err,result)
									{
										if(err)
										{
											console.log("CAL_ID FIND FAILED");
										}
										
										else
										{
											let cps = JSON.parse(calendar.appCalendarPanel("./uploads/" + fileInfo.filename));
											console.log("LENGTH: " + cps.length);
											let counter = 0;
											for(counter = 0; counter < cps.length; counter++)
											{
												let location = "";
												let organizer = "";
												let counter1 = 0;
												for(counter1 = 0; counter1 < cps[counter].propertyList.length; counter1++)
												{
													console.log("PROPNAME: " + cps[counter].propertyList[counter1].propName);
													console.log("PROPDESC: " + cps[counter].propertyList[counter1].propDescr);
													if(cps[counter].propertyList[counter1].propName == "LOCATION")
													{
														location = cps[counter].propertyList[counter1].propDescr;
													}													
													if(cps[counter].propertyList[counter1].propName == "ORGANIZER")
													{
														organizer = cps[counter].propertyList[counter1].propDescr;
													}													
														
												}
												let dateVal = "";
												for(let k = 0; k < cps[counter].startDT.date.length; k++)
												{
													if(k == 4 || k == 6)
														dateVal += '-';
													dateVal += cps[counter].startDT.date[k];
												}

												let timeVal = "";
												for(let m = 0; m < cps[counter].startDT.time.length; m++)
												{
													if(m == 2 || m == 4)
														timeVal += ':';
													timeVal += cps[counter].startDT.time[m];
												}
												let combineVal = dateVal + " " + timeVal;
												if(location == "")
												{
													location = null;
												}					
												if(organizer == "")
												{
													organizer = null;
												}
												let summary = null;
												if(cps[counter].summary != "")
												{
													summary = cps[counter].summary;
												}					
																						
												let eventJSON = [[summary,combineVal,location,organizer,result[0].calFile]];
												db.query('insert into EVENT (summary, start_time, location, organizer, cal_file) values (?)',eventJSON, function(err,result){
													if(err)
													{
														console.log("EVENT ADD FAILED");
													}
												});
												if(cps[counter].alarmList.length > 0)
												{
													for(let counter2 = 0; counter2 < cps[counter].alarmList.length; counter2++)
													{
														let action = "";
														let trigger = "";
														action = cps[counter].alarmList[counter2].action;
														trigger = cps[counter].alarmList[counter2].trigger;
														console.log("ACTION: " + cps[counter].alarmList[counter2].action);
														console.log("TRIGGER: " + cps[counter].alarmList[counter2].trigger);
														let alarmJSON = [[action,trigger,val+1]];
														db.query('insert into ALARM (action, `trigger`, event) values (?)',alarmJSON, function(err,result){
															if(err)
															{
																console.log("ALARM ADD FAILED");
															}
														});
														db.query('SELECT max(event_id) as lastVal FROM EVENT', function(err,result)
														{
															if(err)
															{
																console.log("THE Last Val failed");
															}
														});
													}																																			
												}
												console.log("BLAHBLAH: " + cps[counter].alarmList.length);

/*												db.query('SELECT max(event_id) as lastVal FROM EVENT', function(err,result)
												{
													if(err)
													{
														console.log("THE Last Val failed");
													}
														for(let counter2 = 0; counter2 < cps[counter].alarmList.length; counter2++)
														{
															let action = "";
															let trigger = "";
															action = cps[counter].alarmList[counter2].action;
															trigger = cps[counter].alarmList[counter2].trigger;
															console.log("ACTION: " + cps[counter].alarmList[counter2].action);
															console.log("TRIGGER: " + cps[counter].alarmList[counter2].trigger);
															let alarmJSON = [[action,trigger,result[0].lastVal]];
															db.query('insert into ALARM (action, trigger, event) values (?)',alarmJSON, function(err,result){
																if(err)
																{
																	console.log("ALARM ADD FAILED");
																}
															});
														}																																			
												});
	*/										val++;
											} 
										}
									});
								}
								
							});
						}
					}
				});
			}		
		}
	}
});

app.get('/displayEvt',function(req,res){
	if(db != null && dbCorrect == 1 && storeFlag == 1)
	{
		db.query('SELECT * FROM EVENT ORDER BY start_time DESC',function(err,result){
			if(err)
			{
				console.log("display failed");
			}
			else
			{
				for(let a = 0; a < result.length; a++)
				{
					console.log("DISPLAY HERE: " + JSON.stringify(result[a]));
				}
				res.send(result);
			}
		});
	}
	else if(storeFlag == 0)
	{
		console.log("PLEASE STORE IN THE DATABASE FIRST");
	}	
});

app.get('/fileEvt',function(req,res){
	let inputContent = req.query.field1;
	console.log(inputContent);		
	if(db != null && dbCorrect == 1 && storeFlag == 1 && inputContent != undefined && inputContent != "")
	{
		db.query('SELECT cal_id as calFile FROM FILE WHERE file_name = \"'+inputContent+'\"',function(err,result)
		{
			if(err)
			{
				console.log("CAL_ID FIND FAILED");
			}
			else
			{
				let calFile = result[0].calFile;
				console.log("CAL_ID: " +result[0].calFile);
				db.query('SELECT * FROM EVENT WHERE cal_file = '+calFile,function(err,result){
					if(err)
					{
						console.log("THE EVENTS NOT FOUND");
					}
					else
					{
						console.log("FILE SEL HERE: " + JSON.stringify(result));
						res.send(result);
					}
				});
			}
		});
	}
	else if(storeFlag == 0)
	{
		console.log("PLEASE STORE IN THE DATABASE FIRST");
	}
});

app.get('/conflictEvt',function(req,res){
	if(db != null && dbCorrect == 1 && storeFlag == 1)
	{
		db.query('SELECT DATE_FORMAT(start_time,"%Y-%m-%d %k:%i:%s") as startTime,COUNT(*) as count FROM EVENT GROUP BY start_time HAVING count > 1',function(err,result){
			if(err)
			{
				console.log("CONFLICT FIND END");
			}
			else
			{
				let queryStr = 'SELECT * FROM EVENT WHERE';
				for(let n = 0; n < result.length; n++)
				{
					let count = result[n].count;
					let startTime = result[n].startTime;
					if(n != 0)
					{
						queryStr += ' OR';
					}
					queryStr += ' start_time = \"'+startTime+'\"';
					console.log("COUNT: "+result[n].count);
					console.log("STARTTIME: " + result[n].startTime);
				}
				console.log("QUERYSTR: " +queryStr);
				db.query(queryStr,function(err,result){
					if(err)
					{
						console.log("EVENT FIND FAILED");
					}
					else
					{
						console.log("HERE IT IS>CHEF: "+JSON.stringify(result));
						res.send(result);
					}
				});

			}
		});
	}
	else if(storeFlag == 0)
	{
		console.log("PLEASE STORE IN THE DATABASE FIRST");
	}
});
/*
app.get('/alarmFind',function(req,res){
	let inputContent = req.query.field1;
	console.log(inputContent);
	let cps = JSON.parse(calendar.appCalendarPanel("./uploads/" + inputContent));
	console.log("LENGTH: " + cps.length);
	if(cps.length > 0)
	{
		console.log("YEAH");
//		console.log(cps);
		res.send(cps);
	}	
});*/
/*
app.get('/alarmFind1',function(req,res){
	if(db != null && dbCorrect == 1 && storeFlag == 1)
	{
		let fileName = req.query.field1;
		let eventNo = req.query.field2;
		db.query('SELECT cal_id as calFile FROM FILE WHERE file_name = \"'+fileName+'\"',function(err,result)
		{
			if(err)
			{
				console.log("CAL_ID FIND FAILED");
			}
			else
			{
				let calFile = result[0].calFile;
				console.log("CAL_ID: " +result[0].calFile);
				db.query('SELECT event_id as events FROM EVENT WHERE cal_file = '+calFile,function(err,result){
					if(err)
					{
						console.log("THE EVENTS NOT FOUND");
					}
					else
					{
						let events;
						for(let t = 0; t < result.length; t++)
						{
							if(t == 0)
							{
								events = (result[t].events) - (result[t].events-1);
							}
							else if(t > 0)
							{
								events = result[t].events - result[0].events + 1;
							}
							if(events == eventNo)
							{
								console.log("EVENT NO: " + eventNo);
								console.log("EVENTS: " + events);
							}
						}
					}
				});
			}
		});
	}
	else if(storeFlag == 0)
	{
		console.log("PLEASE STORE IN THE DATABASE FIRST");
	}

});
*/
