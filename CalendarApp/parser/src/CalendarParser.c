/*******************************************
NAME: MAYEZ
STUDENT ID: 0964986
********************************************/
#include "CalendarParser.h"
#include "LinkedListAPI.h"

/** Function to create a Calendar object based on the contents of an iCalendar file.
 *@pre File name cannot be an empty string or NULL.  File name must have the .ics extension.
       File represented by this name must exist and must be readable.
 *@post Either:
        A valid calendar has been created, its address was stored in the variable obj, and OK was returned
		or 
		An error occurred, the calendar was not created, all temporary memory was freed, obj was set to NULL, and the 
		appropriate error code was returned
 *@return the error code indicating success or the error encountered when parsing the calendar
 *@param fileName - a string containing the name of the iCalendar file
 *@param a double pointer to a Calendar struct that needs to be allocated
**/
char* appFilePanel(char* fileName)
{
	printf("APPFILEPANEL\n");
	printf("File name: %s\n",fileName);
	Calendar *calendar;
	calendar->version = 0.0;
	calendar->events = initializeList(&printEvent,&deleteEvent,&compareEvents);
	calendar->properties = initializeList(&printProperty,&deleteProperty,&compareProperties);
	strcpy(calendar->prodID,"");
	char *ret;
	char *info1;
	ICalErrorCode err = createCalendar(fileName,&calendar);
	if(err != OK)
	{
		info1 = printError(err);
		printf("ERROR CODE: '%s'\n",info1);
		free((char*)info1);
		ret = (char*)malloc(sizeof(char) * 
		(strlen("{\"version\":0.0,\"prodID\":\"invalid file\",\"numProps\":2,\"numEvents\":1}") +2));
		sprintf(ret,"{\"version\":0.0,\"prodID\":\"invalid file\",\"numProps\":2,\"numEvents\":1}");

	}
	else
	{
		printf("NO ERRORS HERE\n");
		printf("CAL VER: %lf\n",calendar->version);
		ret = calendarToJSON(calendar);
	}
	return ret;

}

char* appCalendarPanel(char* fileName)
{
	printf("APPCALENDARPANEL\n");
	printf("File name: %s\n",fileName);
	Calendar *calendar;
/*	calendar->version = 0.0;
	calendar->events = initializeList(&printEvent,&deleteEvent,&compareEvents);
	calendar->properties = initializeList(&printProperty,&deleteProperty,&compareProperties);
	strcpy(calendar->prodID,"");*/
	char *ret;
	char *info1;
	ICalErrorCode err = createCalendar(fileName,&calendar);
	if(err != OK)
	{
		info1 = printError(err);
		printf("ERROR CODE: '%s'\n",info1);
		free((char*)info1);
		ret = "[]"; 

	}
	else
	{
		printf("NO ERRORS HERE\n");
		if(calendar->events)
		{
			printf("THERE ARE EVENTS HERE\n");
			ret = eventListToJSON(calendar->events);			
		}
	}
	return ret;

}

char* propertyToJSON(Property* property)
{
	if(property == NULL)
	{
		return "{}";
	}
	char *returning;
	
	returning = (char*)malloc(sizeof(char) * (strlen("{\"propName\":\"") +
	strlen(property->propName) + strlen("\",\"propDescr\":\"") +
	strlen(property->propDescr) + strlen("\"}")+2));
	sprintf(returning,"{\"propName\":\"%s\",\"propDescr\":\"%s\"}",
	property->propName,property->propDescr);	
	return returning;
}

char* propertyListToJSON(List* propertyList)
{
	char *returning;

	if(propertyList)
	{
		Node *properties = propertyList->head;
		Property *p1;
		char *info;
		int length = 0;
		returning = (char*)malloc(sizeof(char) * (strlen("[") +1));
		sprintf(returning,"[");
		length = strlen(returning);		
		while(properties != NULL)
		{
			p1 = properties->data;
			info = propertyToJSON(p1);
			returning = (char*)realloc(returning, sizeof(char) * (length + strlen(info) + 1));
			strcat(returning,info);
			if(properties->next != NULL)
			{
				returning = (char*)realloc(returning, sizeof(char) * (length + strlen(info) + 4));
				strcat(returning,",");				
			}
			length = strlen(returning);
			properties = properties->next;
			free((char*)info);
		}
		returning = (char*)realloc(returning,sizeof(char) * (strlen(returning) + 2));
		strcat(returning,"]");
	}
	return returning;
}

char* alarmToJSON(Alarm* alarm)
{
	if(alarm == NULL)
	{
		return "{}";
	}
	char *returning;
	int propVal = 0;

	if(alarm->properties)
	{
		Node *test2 = alarm->properties->head;
		while(test2 != NULL)
		{
			propVal++;			
			test2 = test2->next;
		}
	}

	returning = (char*)malloc(sizeof(char) * (strlen("{\"action\":\"") +
	strlen(alarm->action) + strlen("\",\"trigger\":\"") +
	strlen(alarm->trigger) + strlen("\",\"numProps\":") +
	sizeof(int) + strlen("}")+2));
	sprintf(returning,"{\"action\":\"%s\",\"trigger\":\"%s\",\"numProps\":%d}",
	alarm->action,alarm->trigger,propVal);	
	return returning;
}

char* alarmListToJSON(List* alarmList)
{
	if(!alarmList || alarmList == NULL)
	{
		return "{}";
	}
	char *returning;

	if(alarmList)
	{
		Node *alarms = alarmList->head;
		Alarm *a1;
		char *info;
		int length = 0;
		returning = (char*)malloc(sizeof(char) * (strlen("[") +1));
		sprintf(returning,"[");
		length = strlen(returning);		
		while(alarms != NULL)
		{
			a1 = alarms->data;
			info = alarmToJSON(a1);
			returning = (char*)realloc(returning, sizeof(char) * (length + strlen(info) + 1));
			strcat(returning,info);
			if(alarms->next != NULL)
			{
				returning = (char*)realloc(returning, sizeof(char) * (length + strlen(info) + 4));
				strcat(returning,",");				
			}
			length = strlen(returning);
			alarms = alarms->next;
			free((char*)info);
		}
		returning = (char*)realloc(returning,sizeof(char) * (strlen(returning) + 2));
		strcat(returning,"]");
	}
	return returning;

}

char* eventToJSON(const Event* event)
{
	if(event == NULL)
	{
		return "{}";
	}
	
	char *startDT;
	char *returning;
	char *sumVal = "";
	int propVal,almVal;
	char *info,*info1;
	
	propVal = 3;
	almVal = 0;
	
	if(event->properties)
	{
		Node *test2 = event->properties->head;
		info = propertyListToJSON(event->properties);
		Property *p1;
		while(test2 != NULL)
		{
			p1 = test2->data;
			if(strcmp(p1->propName,"SUMMARY") == 0)
			{
				sumVal = (char*)malloc(sizeof(char) * 
				(strlen(p1->propDescr)+1));
				strcpy(sumVal,p1->propDescr);
			}
			propVal++;			
			test2 = test2->next;
		}
		
	}

	if(event->alarms)
	{
		Node *test3 = event->alarms->head;
		info1 = alarmListToJSON(event->alarms);
		while(test3 != NULL)
		{
			almVal++;
			test3 = test3->next;
		}
	}
	
	startDT = dtToJSON(event->startDateTime);
	returning = (char*)malloc(sizeof(char) * (strlen("{\"startDT\":") +
	strlen(startDT) + strlen(",\"numProps\":") +
	sizeof(int) + strlen(",\"numAlarms\":") +
	sizeof(int) + strlen(",\"summary\":\"") +
	strlen(sumVal) +  strlen("\",\"alarmList\":") +
	strlen(info1)  +  strlen(",\"propertyList\":") + 
	strlen(info) + strlen("}")+2));
	sprintf(returning,"{\"startDT\":%s,\"numProps\":%d,\"numAlarms\":%d,\"summary\":\"%s\",\"alarmList\":%s,\"propertyList\":%s}",
	startDT,propVal,almVal,sumVal,info1,info);

	if(strlen(sumVal) > 0)
		free((char*)sumVal);
	if(strlen(startDT) > 0)
		free((char*)startDT);
	return returning;
}

ICalErrorCode createCalendar(char* fileName, Calendar** obj)
{
	FILE *fp;
	char line[2000];
	char fileCheck[256];
	char *strSplit;
	char *dateSplit;
	char *fileSplit;
	char tempStr[2000];	
	ICalErrorCode err;
	Event *events;
	Alarm *alarms;
	int i = 0;
	if(fileName != NULL)
	{
		fp = fopen(fileName,"r");
	}
	else
	{
		err = INV_FILE;
		return err;
	}
	char property[1000];
	char data[2000] = "";
	char dateParse[2000] = "";
	char dateCopy[2000] = "";
	int eventFlag = 0;
	int alarmFlag = 0;

	int versionFlag = 0;
	int prodIDFlag = 0;
	int calFlag = 0;
	int eFlag = 0;
	int uidFlag = 0;
	int createDate = 0;
	int aFlag = 0;
	int actionFlag = 0;
	int triggerFlag = 0;
	int eventOccur = 0;
	if(fp == NULL)
	{
		err = INV_FILE;
		return err;
	}
	if(fileName == NULL || strcmp(fileName,"") == 0)
	{
		if(fileName != NULL)
		{
			fclose(fp);
		}
		err = INV_FILE;
		return err;
	}
	if(!obj)
	{
		fclose(fp);
		err = INV_CAL;
		return err;		
	}

	strcpy(fileCheck,fileName);
	fileSplit = strtok(fileCheck,".");

	if(fileSplit != NULL)
	{
		fileSplit = strtok(NULL,".");
	}

	if(strcmp(fileSplit,"ics") != 0)
	{
		fclose(fp);
		err = INV_FILE;
		return err;
	}

	if(fgets(line,sizeof(line),fp) != NULL)
	{
		if(line[strlen(line) - 2] == '\r' && line[strlen(line) -1] == '\n')
		{
			line[strlen(line)-2] = '\0';
		}
	
		if(strcmp(line,"BEGIN:VCALENDAR") != 0)
		{
			fclose(fp);
			fp = fopen(fileName,"r");
			while(fgets(line,sizeof(line),fp) != NULL)
			{
				if(line[strlen(line) - 2] == '\r' && line[strlen(line) -1] == '\n')
				{
					line[strlen(line)-2] = '\0';
				}

				if(line[0] == ';')
				{
					continue;
				}
				
				else if(strcmp(data,"") != 0 && (line[0] != ' ' && line[0] != '\t'))
				{
					strcpy(data,"");
				}
				
				strcpy(tempStr,line);
			
				if(strcmp(data,"") != 0 && (line[0] == ' ' || line[0] == '\t'))
				{
					strSplit = strtok(tempStr,"\r\n");
			
					strSplit = strtok(strSplit,"\r\n");

					if(line[0] == ' ')	
					{
						if(strSplit[0] == ' ')
						{
							for(i = 0; i < strlen(strSplit); i++)
							{
								strSplit[i] = strSplit[i+1];
							}
						}
					}

					else if(line[0] == '\t')
					{
						if(strSplit[0] == '\t')
						{
							for(i = 0; i < strlen(strSplit); i++)
							{
							
								strSplit[i] = strSplit[i+1];
						
							}
						}	
					}
					strcat(data,strSplit);
					if(strcmp(data,"BEGIN:VCALENDAR") == 0)
					{
						break;
					}
					continue;	
				}

				else if((line[0] != ' ' && line[0] != '\t') && strcmp(data,"") == 0)
				{
					strSplit = strtok(tempStr,":;\r\n");
					strcpy(property,strSplit);
					while(strSplit != NULL)
					{
						strcpy(data,strSplit);
						strSplit = strtok(NULL,"\r\n");
					}
					continue;
				}
								
			}
			if(strcmp(data,"BEGIN:VCALENDAR") != 0)
			{
				fclose(fp);
				err = INV_CAL;
				return err;		
			}
			else
			{
				calFlag = 1;
				strcpy(data,"");
			}
		}
		else
		{
			calFlag = 1;
		}				
	
	}

	while(fgets(line,sizeof(line),fp) != NULL)
	{
		if(line[strlen(line) - 2] == '\r' && line[strlen(line) -1] == '\n')
		{
			line[strlen(line)-2] = '\0';
		}

		if(line[0] == ';')
		{
			continue;
		}
		else if(strcmp(line,"") == 0)
		{
			fclose(fp);
			err = INV_CAL;
			return err;					
		}

		else if(strcmp(data,"") != 0 && (line[0] != ' ' && line[0] != '\t'))
		{
	
			if(strcmp("VERSION",property) == 0)
			{
				if(versionFlag == 1)
				{
					fclose(fp);
					err = DUP_VER;
					return err;
				}

				if(strcmp(data,property) == 0 || atof(data) == 0.0)
				{
					fclose(fp);
					err = INV_VER;
					return err;
				}
				versionFlag = 1;
			}

			else if(strcmp("PRODID",property) == 0)
			{
				if(prodIDFlag == 1)
				{
					fclose(fp);
					err = DUP_PRODID;
					return err;
				}

				if(strcmp(data,property) == 0)
				{
					fclose(fp);
					err = INV_PRODID;
					return err;
				}
				prodIDFlag = 1;
			}

			if(strcmp(data,property) == 0 && eFlag == 0)
			{
				fclose(fp);
				err = INV_CAL;
				return err;
			}

			else if(eFlag == 1)
			{
				if(strcmp("BEGIN",property) == 0 && strcmp("VEVENT",data) == 0)
				{
					fclose(fp);
					err = INV_EVENT;
					return err;
				}
				if(strcmp("UID",property) == 0)
				{
					if(strcmp(data,property) == 0)
					{
						fclose(fp);
						err = INV_EVENT;
						return err;
					}

					uidFlag = 1;				
				}
				else if(strcmp("DTSTAMP",property) == 0)
				{
					if(strcmp(data,property) == 0)
					{
						fclose(fp);
						err = INV_DT;
						return err;
					}
					
					char data1[1000] = "";
					strcpy(data1,data);
					strtok(data1,"T");
					if(strlen(data1) != 8)
					{
						fclose(fp);
						err = INV_DT;
						return err;						
					}
					char *test;
					test = strchr(data,'T');
					if(test == NULL)
					{
						fclose(fp);
						err = INV_DT;
						return err;
					}
					createDate = 1;		
				}	
				else if(aFlag == 1)
				{
					if(strcmp("BEGIN",property) == 0 && strcmp("VALARM",data) == 0)
					{
						fclose(fp);
						err = INV_ALARM;
						return err;
					}

					if(strcmp("ACTION",property) == 0)
					{
						if(strcmp(data,property) == 0)
						{
							fclose(fp);
							err = INV_ALARM;
							return err;
						}
						actionFlag = 1;
					}	

					else if(strcmp("TRIGGER",property) == 0)
					{
						if(strcmp(data,property) == 0)
						{
							fclose(fp);
							err = INV_ALARM;
							return err;
						}
						triggerFlag = 1;
					}	

					else if(strcmp(data,property) == 0)
					{
						fclose(fp);
						err = INV_ALARM;
						return err;
					}

				}					
								
				if(strcmp("BEGIN",property) == 0 && strcmp("VALARM",data) == 0)
				{
					if(versionFlag == 0 || prodIDFlag == 0)
					{
						fclose(fp);
						err = INV_CAL;
						return err;			
					}
					if(uidFlag == 0 || createDate == 0)
					{
						fclose(fp);
						err = INV_EVENT;
						return err;	
					}

					aFlag = 1;
				}
				
				else if(strcmp(data,property) == 0 && eFlag == 1)
				{
					fclose(fp);
					/*A2 test 8 errorcode location*/
					err = INV_EVENT;
					return err;
				}

				
			}

			if(strcmp("BEGIN",property) == 0 && strcmp("VEVENT",data) == 0)
			{
				if(versionFlag == 0 || prodIDFlag == 0)
				{
					fclose(fp);
					err = INV_CAL;
					return err;			
				}
				eFlag = 1;
				eventOccur = 1;
			}
			strcpy(data,"");
		}	

		if(strcmp("END:VCALENDAR",line) == 0)
		{
			calFlag = 0;
		}	
		
		else if(strcmp("END:VALARM",line) == 0)
		{
			if(aFlag == 1)
			{
				if(actionFlag == 0 || triggerFlag == 0)
				{
					fclose(fp);
					err = INV_ALARM;
					return err;
				}
				actionFlag = 0;
				triggerFlag = 0;
				aFlag = 0;
			}
			else if(aFlag == 0)
			{
				if(versionFlag == 0 || prodIDFlag == 0)
				{
					fclose(fp);
					err = INV_CAL;
					return err;			
				}


				if(uidFlag == 0 || createDate == 0)
				{
					fclose(fp);
					err = INV_EVENT;
					return err;	
				}

				fclose(fp);
				err = INV_ALARM;
				return err;	
			}
		}

		else if(strcmp("END:VEVENT",line) == 0)
		{
			if(eFlag == 1)
			{

				if(uidFlag == 0 || createDate == 0)
				{
					fclose(fp);
					err = INV_EVENT;
					return err;	
				}
				uidFlag = 0;
				createDate = 0;
				eFlag = 0;
			}
			else if(eFlag == 0)
			{
				if(versionFlag == 0 || prodIDFlag == 0)
				{
					fclose(fp);
					err = INV_CAL;
					return err;			
				}

				fclose(fp);
				err = INV_EVENT;
				return err;
			}
		}

		strcpy(tempStr,line);
	
		if(strcmp(data,"") != 0 && (line[0] == ' ' || line[0] == '\t'))
		{
			strSplit = strtok(tempStr,"\r\n");
	
			strSplit = strtok(strSplit,"\r\n");

			if(line[0] == ' ')	
			{
				if(strSplit[0] == ' ')
				{
    				for(i = 0; i < strlen(strSplit); i++)
					{
						strSplit[i] = strSplit[i+1];
					}
				}
			}

			else if(line[0] == '\t')
			{
				if(strSplit[0] == '\t')
				{
	
					for(i = 0; i < strlen(strSplit); i++)
					{
					
						strSplit[i] = strSplit[i+1];
				
					}
				}	
			}
			strcat(data,strSplit);
			continue;	
		}

		else if((line[0] != ' ' && line[0] != '\t') && strcmp(data,"") == 0)
		{

			strSplit = strtok(tempStr,":;\r\n");
			strcpy(property,strSplit);
			while(strSplit != NULL)
			{
				strcpy(data,strSplit);
				strSplit = strtok(NULL,"\r\n");
			}
			continue;
		}
		
	}

	if(aFlag == 1)
	{
		if(actionFlag == 0 || triggerFlag == 0)
		{
			fclose(fp);
			err = INV_ALARM;
			return err;
		}

		fclose(fp);
		err = INV_ALARM;
		return err;			
	}

	if(eFlag == 1)
	{
		if(versionFlag == 0 || prodIDFlag == 0)
		{
			fclose(fp);
			err = INV_CAL;
			return err;			
		}

		if(uidFlag == 0 || createDate == 0)
		{
			fclose(fp);
			err = INV_EVENT;
			return err;	
		}

		if((actionFlag == 0 || triggerFlag == 0) && aFlag == 1)
		{
			fclose(fp);
			err = INV_ALARM;
			return err;
		}

		fclose(fp);
		err = INV_EVENT;
		return err;
	}

	if(calFlag == 1 || versionFlag == 0 || prodIDFlag == 0)
	{
		fclose(fp);
		err = INV_CAL;
		return err;			
	}
	
	if(eventOccur == 0)
	{
		fclose(fp);
		err = INV_CAL;
		return err;				
	}
	
	strcpy(data,"");
	strcpy(property,"");
	fclose(fp);

	fp = fopen(fileName,"r");
	Calendar *calendar = (Calendar*)malloc(sizeof(Calendar));
	calendar->version = 0.0;
	calendar->events = initializeList(&printEvent,&deleteEvent,&compareEvents);
	calendar->properties = initializeList(&printProperty,&deleteProperty,&compareProperties);
	strcpy(calendar->prodID,"");

	while(fgets(line, sizeof(line), fp) != NULL)
	{
		if(line[strlen(line) - 2] == '\r' && line[strlen(line) -1] == '\n')
		{
			line[strlen(line)-2] = '\0';
		}
		if(line[0] == ';')
		{
			continue;
		}

		else if(strcmp(data,"") != 0 && (line[0] != ' ' && line[0] != '\t'))
		{
			char *blah;
			blah = strstr(data,property);
			if(blah != NULL)
			{
					char testing[1000] = "";
					strcpy(testing,data);
					strtok(testing,":;");
					while(strSplit != NULL)
					{
						strcpy(data,strSplit);
						strSplit = strtok(NULL,"\r\n");
					}
					
			}
			if(strcmp(property,"BEGIN") == 0 && strcmp(data,"VEVENT") == 0)
			{
				eventFlag = 1;		
			}			
	
			if(strcmp(property,"BEGIN") == 0 && strcmp(data,"VALARM") == 0)
			{
				alarmFlag = 1;	
			}

			if(strcmp(property,"VERSION") == 0)
			{
				if(strcmp(data,property) == 0)
				{
					fclose(fp);
					deleteCalendar(calendar);
					err = INV_VER;
					return err;
				}	
				calendar->version = atof(data);
			}
			else if(strcmp(property,"PRODID") == 0)
			{
				strcpy(calendar->prodID,data);
			}

			else if(strcmp(property,"METHOD") == 0 ||
			strcmp(property,"CALSCALE") == 0)
			{
				Property *node = (Property*)malloc(sizeof(Property) + (sizeof(char) * strlen(property) +1) + (sizeof(char) * (strlen(data) + 1)));
		
				strcpy(node->propName,property);
				strcpy(node->propDescr,data);
				insertBack(calendar->properties,node);	
			}
			
			if(eventFlag == 1)
			{
				if(strcmp("BEGIN",property) == 0 && strcmp("VEVENT",data) == 0)
				{
					events = (Event*)malloc(sizeof(Event)); 
					strcpy(events->UID,"");
					strcpy(events->creationDateTime.date,"");
					strcpy(events->creationDateTime.time,"");
					strcpy(events->startDateTime.date,"");
					strcpy(events->startDateTime.time,"");	
					events->properties = initializeList(&printProperty,&deleteProperty,&compareProperties);
					events->alarms = initializeList(&printAlarm,&deleteAlarm,&compareAlarms);

				}

				if(strcmp(property,"END") == 0 && strcmp(data,"VEVENT") == 0)
				{
					eventFlag = 0;
					insertBack(calendar->events,events);
				}
				
				if(strcmp(property,"UID") == 0 && alarmFlag == 0)
				{
					strcpy(events->UID,data);
				}

				else if(strcmp(property,"DTSTAMP") == 0 || strcmp(property,"DTSTART") == 0)
				{
					strcpy(dateCopy,data);
					dateSplit = dateCopy;	
					dateSplit = strtok(dateSplit,":");	
					while(dateSplit != NULL)
					{
						strcpy(dateParse,dateSplit);
						dateSplit = strtok(NULL,":");
					}
				
					strcpy(dateCopy,"");
					dateSplit = dateParse;
					dateSplit = strtok(dateSplit,"T");
					DateTime dt;
					strcpy(dt.date,dateParse);	

					while(dateSplit != NULL)
					{
						strcpy(dateCopy,dateSplit);
						dateSplit = strtok(NULL,"\r\n");
					}
					if(dateCopy[6] == 'Z')
					{
						dt.UTC = 1;
						dateCopy[6] = '\0';
					}
					else if(dateCopy[6] != 'Z')
					{
						dt.UTC = 0;
					}
					strcpy(dt.time,dateCopy);

					if(strcmp(property,"DTSTAMP") == 0)
					{
						events->creationDateTime = dt;
					}
					else if(strcmp(property,"DTSTART") == 0)
					{
						events->startDateTime = dt;
					}
				}

				else if(alarmFlag == 1)
				{
					if(strcmp(property,"END") == 0 && strcmp(data,"VALARM") == 0)
					{
						alarmFlag = 0;
						insertBack(events->alarms,alarms);			
					}		
					if(strcmp("BEGIN",property) == 0 && strcmp("VALARM",data) == 0)
					{
						alarms = (Alarm*)malloc(sizeof(Alarm));
						strcpy(alarms->action,"");
						alarms->trigger = NULL;
						alarms->properties = initializeList(&printAlarm,&deleteAlarm,&compareAlarms);		
					}

					if(strcmp(property,"ACTION") == 0)
					{
						strcpy(alarms->action,data);
					}

					else if(strcmp(property,"TRIGGER") == 0)
					{
						alarms->trigger = (char*)malloc(sizeof(char) * strlen(data) + 1);
						strcpy(alarms->trigger,data);
					}

					else if(strcmp(data,"VALARM") != 0)
					{
						Property *props = (Property*)malloc(sizeof(Property) + (sizeof(char) * strlen(property) + 1) + (sizeof(char) * (strlen(data) + 1)));
						strcpy(props->propName,property);
						strcpy(props->propDescr,data);		
						insertBack(alarms->properties,props);
					}

				}
				
				else if(strcmp(data,"VEVENT") != 0)
				{
					Property *other = (Property*)malloc(sizeof(Property) + (sizeof(char) * strlen(property) + 1) + (sizeof(char) * (strlen(data) + 1)));
					strcpy(other->propName,property);
					strcpy(other->propDescr,data);

					insertBack(events->properties,other);
				}


			}		
			strcpy(data,"");
		}
		strcpy(tempStr,line);
	
		if(strcmp(data,"") != 0 && (line[0] == ' ' || line[0] == '\t'))
		{
			strSplit = strtok(tempStr,"\r\n");
			strSplit = strtok(strSplit,"\r\n");

			if(line[0] == ' ')	
			{
				if(strSplit[0] == ' ')
				{
					for(i = 0; i < strlen(strSplit); i++)
					{
						strSplit[i] = strSplit[i+1];
					}
				}
			}

			else if(line[0] == '\t')
			{
				if(strSplit[0] == '\t')
				{
					for(i = 0; i < strlen(strSplit); i++)
					{
					
						strSplit[i] = strSplit[i+1];
				
					}
				}	
			}
			strcat(data,strSplit);
			continue;	
		}

		else if((line[0] != ' ' && line[0] != '\t') && strcmp(data,"") == 0)
		{
			strSplit = strtok(tempStr,":;\r\n");
			strcpy(property,strSplit);
			while(strSplit != NULL)
			{
				strcpy(data,strSplit);
				strSplit = strtok(NULL,"\r\n");
			}
			continue;
		}
		
	}

	fclose(fp);
	*obj = calendar;
	err = OK;
	return err;
		
}

/** Function to delete all calendar content and free all the memory.
 *@pre Calendar object exists, is not null, and has not been freed
 *@post Calendar object had been freed
 *@return none
 *@param obj - a pointer to a Calendar struct
**/
void deleteCalendar(Calendar* obj)
{
	if(!obj)
	{
		return;
	}
	if(obj->properties)
	{
		Node *test = obj->properties->head;
		Property *p1;
		while(test != NULL)
		{
			p1 = (Property*)test->data;	
			test = test->next;
			free(p1);
		}
		freeList(obj->properties);
	}

	if(obj->events)
	{
		Node *test1 = obj->events->head;	 //Print Calendar->events and its properties
		Event* e1;
		while(test1 != NULL)
		{
			e1 = (Event*)test1->data;
			if(e1->properties)
			{
				Node *test2 = e1->properties->head;
				Property *p1;
				while(test2 != NULL)
				{
					p1 = (Property*)test2->data;
					test2 = test2->next;
					free(p1);
				}
				freeList(e1->properties);
			}

			if(e1->alarms)
			{
				Node *test3 = e1->alarms->head;
				Alarm *a1;
				while(test3 != NULL)
				{
					a1 = (Alarm*)test3->data;
					if(a1->trigger)
					{
						free(a1->trigger);
					}
					if(a1->properties)
					{
						Node *test4 = a1->properties->head;
						Property *p2;
						if(test4 != NULL)
						{
							while(test4 != NULL)
							{
								p2 = (Property*)test4->data;
								test4 = test4->next;
								free(p2);
							}	
							freeList(a1->properties);
						}
					}
					test3 = test3->next;
					free(a1);
                                     
				}
				freeList(e1->alarms);
			}
			test1 = test1->next;
			free(e1);
		}
	}

	if(obj->events)
	{
		freeList(obj->events);
	}
	free(obj);
	
}


/** Function to create a string representation of a Calendar object.
 *@pre Calendar object exists, is not null, and is valid
 *@post Calendar has not been modified in any way, and a string representing the Calndar contents has been created
 *@return a string contaning a humanly readable representation of a Calendar object
 *@param obj - a pointer to a Calendar struct
**/
char* printCalendar(const Calendar* obj)
{
	if(!obj)
	{
		return NULL;
	}
	char * returning;
	int length = 0;
	
	returning = (char*)malloc(sizeof(char) * (strlen("Calendar Struct: ") +
	+ strlen("\nVERSION: ") + sizeof(float) +  strlen("\nPRODID: ") +
	strlen(obj->prodID) + 2));
 	sprintf(returning,"Calendar Struct: \nVERSION: %.1f\nPRODID: %s\n"
 	,obj->version,obj->prodID);
	
	length = strlen(returning);
	if(obj->properties)
	{
		Node *test = obj->properties->head;
		Property *p1;
		char *info; 
		while(test != NULL)
		{
			p1 = (Property*)test->data;	
			info = printProperty(p1);
			returning = (char*)realloc(returning, sizeof(char) * (length + strlen(info) + 1));
			strcat(returning,info);
			length = strlen(returning);
			test = test->next;
			free(info);
		}
		
	}

	if(obj->events)
	{
		Node *test1 = obj->events->head;	 //Print Calendar->events and its properties
		Event* e1;
		char *info;
		while(test1 != NULL)
		{
			e1 = (Event*)test1->data;
			info = printEvent(e1);
			returning = (char*)realloc(returning, sizeof(char) * (length + strlen(info) + 1));
			strcat(returning,info);
			length = strlen(returning);
			if(e1->properties)
			{
				Node *test2 = e1->properties->head;
				Property *p1;
				char *info1;
				while(test2 != NULL)
				{
					p1 = (Property*)test2->data;
					info1 = printProperty(p1);
					returning = (char*)realloc(returning, sizeof(char) * (length + strlen(info1) + 1));
					strcat(returning,info1);
					length = strlen(returning);
					test2 = test2->next;
					free(info1);
				}
			}
			
			if(e1->alarms)
			{
				Node *test3 = e1->alarms->head;
				Alarm *a1;
				char *info2;
				while(test3 != NULL)
				{
					a1 = (Alarm*)test3->data;
					info2 = printAlarm(a1);
					returning = (char*)realloc(returning, sizeof(char) * (length + strlen(info2) + 1));
					strcat(returning,info2);
					length = strlen(returning);
					if(a1->properties)
					{
						Node *test4 = a1->properties->head;
						Property *p2;
						char *info3;
						while(test4 != NULL)
						{
							p2 = (Property*)test4->data;
							info3 = printProperty(p2);
							returning = (char*)realloc(returning, sizeof(char) * (length + strlen(info3) + 1));
							strcat(returning,info3);
							length = strlen(returning);
							test4 = test4->next;
							free(info3);
						}
					}
					test3 = test3->next;
					free(info2);
				}
			}
			
			test1 = test1->next;
			free(info);
		}
	}
	return returning;

}


/** Function to "convert" the ICalErrorCode into a humanly redabale string.
 *@return a string contaning a humanly readable representation of the error code by indexing into 
          the descr array using rhe error code enum value as an index
 *@param err - an error code
**/
char* printError(ICalErrorCode err)
{
	char *error;
	char line[200];
	if(err == OK)
	{
		strcpy(line,"There are no issues");
	}
	else if(err == INV_FILE)
	{
		strcpy(line,"The file chosen is an invalid file");
	}
	else if(err == INV_CAL)
	{
		strcpy(line,"The calendar object in the file is invalid");	
	}	
	else if(err == INV_VER)
	{
		strcpy(line,"The version property is present but, its incorrect");
	} 
	else if(err == DUP_VER)
	{
		strcpy(line,"The version property has appeared more than once");
	}
	else if(err == INV_PRODID)
	{
		strcpy(line,"The product id is present but, its incorrect");
	}
	else if(err == DUP_PRODID)
	{
		strcpy(line,"The product id has appeared more than once");
	}
	else if(err == INV_EVENT)
	{
		strcpy(line,"The event is present but, incorrect in some way");
	}
	else if(err == INV_DT)
	{
		strcpy(line,"Either the date or time properties are incorrect");
	}
	else if(err == INV_ALARM)
	{
		strcpy(line,"The alarm is present but, incorrect in some way");
	}
	else if(err == OTHER_ERROR)
	{
		strcpy(line,"There is some other type of error that has occurred");
	}
	else if(err == WRITE_ERROR)
	{
		strcpy(line,"There was an error writing to a file");		
	}
	error = malloc(sizeof(char) * (strlen(line) + 1));
	strcpy(error,line);
	return error;
}


/** Function to writing a Calendar object into a file in iCalendar format.
 *@pre Calendar object exists, is not null, and is valid
 *@post Calendar has not been modified in any way, and a file representing the
        Calendar contents in iCalendar format has been created
 *@return the error code indicating success or the error encountered when parsing the calendar
 *@param obj - a pointer to a Calendar struct
 **/
ICalErrorCode writeCalendar(char* fileName, const Calendar* obj)
{
	FILE *fp;
	ICalErrorCode err;
	if(!fileName)
	{
		err = WRITE_ERROR;
		return err;
	}
	
	if(!obj)
	{
		err = WRITE_ERROR;
		return err;
	}

	if(fileName != NULL)
	{
		fp = fopen(fileName,"w");
	}
	else
	{
		err = WRITE_ERROR;
		return err;
	}

	if(fp == NULL)
	{
		free((Calendar*)obj);
		fclose(fp);
		err = WRITE_ERROR;
		return err;
	}
	
	if(fileName == NULL || strcmp(fileName,"") == 0)
	{
		if(fileName != NULL)
		{
			fclose(fp);
		}
		free((Calendar*)obj);
		err = WRITE_ERROR;
		return err;
	}

	if(!obj)
	{
		fclose(fp);
		free((Calendar*)obj);
		err = WRITE_ERROR;
		return err;		
	}
	fprintf(fp,"BEGIN:VCALENDAR\r\nVERSION:%.1f\r\nPRODID:%s\r\n",obj->version,obj->prodID);
	if(obj->properties)
	{
		Node *test = obj->properties->head;
		Property *p1;
		while(test != NULL)
		{
			p1 = test->data;
			int equalFlag = 0;
			int i = 0;
			for(i = 0; i < strlen(p1->propDescr); i++)
			{
				if(p1->propDescr[i] == '=')
				{
					equalFlag = 1;
				}
			}
			if(equalFlag == 0)
			{
				fprintf(fp,"%s:%s\r\n",p1->propName,p1->propDescr);
			}
			else
			{
				fprintf(fp,"%s;%s\r\n",p1->propName,p1->propDescr);
			}				
			test = test->next;
		}
		
	}

	if(obj->events)
	{
		Node *test1 = obj->events->head;	 //Print Calendar->events and its properties
		Event* e1;
		while(test1 != NULL)
		{
			fprintf(fp,"BEGIN:VEVENT\r\n");
			e1 = (Event*)test1->data;
			fprintf(fp,"UID:%s\r\n",e1->UID);
			fprintf(fp,"DTSTAMP:%sT%s",e1->creationDateTime.date,e1->creationDateTime.time);
			if(e1->creationDateTime.UTC == true)
			{
				fprintf(fp,"Z\r\n");		
			}
			else
			{
				fprintf(fp,"\r\n");
			}
			
			fprintf(fp,"DTSTART:%sT%s",e1->startDateTime.date,e1->startDateTime.time);
			if(e1->startDateTime.UTC == true)
			{
				fprintf(fp,"Z\r\n");		
			}
			else
			{
				fprintf(fp,"\r\n");
			}
			
			if(e1->properties)
			{
				Node *test2 = e1->properties->head;
				Property *p1;
				while(test2 != NULL)
				{
					p1 = test2->data;
					int equalFlag = 0;
					int i = 0;
					for(i = 0; i < strlen(p1->propDescr); i++)
					{
						if(p1->propDescr[i] == '=')
						{
							equalFlag = 1;
						}
					}
					if(equalFlag == 0)
					{
						fprintf(fp,"%s:%s\r\n",p1->propName,p1->propDescr);
					}
					else
					{
						fprintf(fp,"%s;%s\r\n",p1->propName,p1->propDescr);
					}				
					test2 = test2->next;
				}
			}
			
			if(e1->alarms)
			{
				Node *test3 = e1->alarms->head;
				Alarm *a1;
				while(test3 != NULL)
				{
					fprintf(fp,"BEGIN:VALARM\r\n");
					a1 = test3->data;
					fprintf(fp,"ACTION:%s\r\n",a1->action);
					fprintf(fp,"TRIGGER");
					int equalFlag = 0;
					int i = 0;
					for(i = 0; i < strlen(a1->trigger); i++)
					{
						if(a1->trigger[i] == '=')
						{
							equalFlag = 1;
						}
					}
					if(equalFlag == 0)
					{
						fprintf(fp,":%s\r\n",a1->trigger);
					}
					else
					{
						fprintf(fp,";%s\r\n",a1->trigger);
					}				

					if(a1->properties)
					{
						Node *test4 = a1->properties->head;
						Property *p2;
						while(test4 != NULL)
						{
							p2 = test4->data;
							int equalFlag = 0;
							int i = 0;
							for(i = 0; i < strlen(p2->propDescr); i++)
							{
								if(p2->propDescr[i] == '=')
								{
									equalFlag = 1;
								}
							}
							if(equalFlag == 0)
							{
								fprintf(fp,"%s:%s\r\n",p2->propName,p2->propDescr);
							}
							else
							{
								fprintf(fp,"%s;%s\r\n",p2->propName,p2->propDescr);
							}				
							test4 = test4->next;
						}
					}
					test3 = test3->next;
					fprintf(fp,"END:VALARM\r\n");
				}
			}
			
			test1 = test1->next;
			fprintf(fp,"END:VEVENT\r\n");
		}
	}

	fprintf(fp,"END:VCALENDAR\r\n");
	fclose(fp);
	err = OK;
	return err;
}


/** Function to validating an existing a Calendar object
 *@pre Calendar object exists and is not null
 *@post Calendar has not been modified in any way
 *@return the error code indicating success or the error encountered when validating the calendar
 *@param obj - a pointer to a Calendar struct
 **/
ICalErrorCode validateCalendar(const Calendar* obj)
{
	/* potential issues to fix:
	 * 1. same property in different component (ex. ATTACH in evtproperties vs ATTACH in almprop
	 * 2. A List being NULL or not existing to begin with
	 */
	if(obj == NULL)
	{
		return INV_CAL;
	}
	if(!obj->events || !obj->properties)
	{
		return INV_CAL;
	}

	if(!obj || strcmp(obj->prodID,"") == 0 || obj->version == 0.0)
	{
		return INV_CAL;
	}
		
	if(obj->properties)
	{
		int invProp = 0;
		int methodFlag = 0;
		int calscaleFlag = 0;

		Node *test = obj->properties->head;
		Property *p1;
		while(test != NULL)
		{
			p1 = test->data;
			if(strcmp(p1->propName,"CALSCALE") != 0 && strcmp(p1->propName,"METHOD") != 0)
			{
				invProp = 1;
			}
			if(strcmp(p1->propName,"CALSCALE") == 0)
			{
				calscaleFlag++;
			}
			if(strcmp(p1->propName,"METHOD") == 0)
			{
				methodFlag++;
			}
			
			if(invProp == 1 || methodFlag > 1 || calscaleFlag > 1 || strcmp(p1->propDescr,"") == 0
			|| !p1->propDescr)
			{
				return INV_CAL;
			}

			test = test->next;
		}	
	}
   
    if(obj->properties == NULL || obj->events == NULL)
	{
		return INV_CAL;
	}
	
	if(obj->events)
	{
		Node *test1 = obj->events->head;	 		
		Event* e1;
		while(test1 != NULL)
		{
			e1 = test1->data;
			if(!e1->properties || strcmp(e1->UID,"") == 0 || strlen(e1->creationDateTime.date) != 8
			|| strlen(e1->creationDateTime.time) != 6 || e1->properties == NULL) /*e1->alarms == NULL*/
			{
				return INV_EVENT;
			}
	
			if(e1->properties)
			{
				int eventPropOpt[18];
				eventPropOpt[0] = 0;  //CLASS 
				eventPropOpt[1] = 0;  //CREATED 
				eventPropOpt[2] = 0;  //DESCRIPTION 
				eventPropOpt[3] = 0;  //GEO 
				eventPropOpt[4] = 0;  //LAST-MODIFIED
				eventPropOpt[5] = 0;  //LOCATION
				eventPropOpt[6] = 0;  //ORGANIZER
				eventPropOpt[7] = 0;  //PRIORITY
				eventPropOpt[8] = 0;  //SEQUENCE
				eventPropOpt[9] = 0;  //STATUS
				eventPropOpt[10] = 0; //SUMMARY
				eventPropOpt[11] = 0; //TRANSP
				eventPropOpt[12] = 0; //URL
				eventPropOpt[13] = 0; //RECURRENCE-ID
				eventPropOpt[14] = 0; //RRULE
				eventPropOpt[15] = 0; //DTSTART
				eventPropOpt[16] = 0; //DTEND
				eventPropOpt[17] = 0; //DURATION

				Node *test2 = e1->properties->head;
				Property *p1;
				while(test2 != NULL)
				{
					p1 = test2->data;
					if(strcmp(p1->propName,"CLASS") == 0)
						eventPropOpt[0]++;

					else if(strcmp(p1->propName,"CREATED") == 0)
						eventPropOpt[1]++;

					else if(strcmp(p1->propName,"DESCRIPTION") == 0)
						eventPropOpt[2]++;

					else if(strcmp(p1->propName,"GEO") == 0)
						eventPropOpt[3]++;

					else if(strcmp(p1->propName,"LAST-MODIFIED") == 0)
						eventPropOpt[4]++;

					else if(strcmp(p1->propName,"LOCATION") == 0)
						eventPropOpt[5]++;

					else if(strcmp(p1->propName,"ORGANIZER") == 0)
						eventPropOpt[6]++;

					else if(strcmp(p1->propName,"PRIORITY") == 0)
						eventPropOpt[7]++;

					else if(strcmp(p1->propName,"SEQUENCE") == 0)
						eventPropOpt[8]++;

					else if(strcmp(p1->propName,"STATUS") == 0)
						eventPropOpt[9]++;

					else if(strcmp(p1->propName,"SUMMARY") == 0)
						eventPropOpt[10]++;

					else if(strcmp(p1->propName,"TRANSP") == 0)
						eventPropOpt[11]++;

					else if(strcmp(p1->propName,"URL") == 0)
						eventPropOpt[12]++;

					else if(strcmp(p1->propName,"RECURRENCE-ID") == 0)
						eventPropOpt[13]++;

					else if(strcmp(p1->propName,"RRULE") == 0)
						eventPropOpt[14]++;

					else if(strcmp(p1->propName,"DTSTART") == 0)
						eventPropOpt[15]++;

					else if(strcmp(p1->propName,"DTEND") == 0)
						eventPropOpt[16]++;

					else if(strcmp(p1->propName,"DURATION") == 0)
						eventPropOpt[17]++;
					
					else if(strcmp(p1->propName,"ATTACH") != 0 && strcmp(p1->propName,"ATTENDEE") != 0 &&
					strcmp(p1->propName,"CATEGORIES") != 0 && strcmp(p1->propName,"COMMENT") != 0 &&	
					strcmp(p1->propName,"CONTACT") != 0 && strcmp(p1->propName,"EXDATE") != 0 &&					
					strcmp(p1->propName,"REQUEST-STATUS") != 0 && strcmp(p1->propName,"RELATED-TO") != 0 &&					
					strcmp(p1->propName,"RESOURCES") != 0 && strcmp(p1->propName,"RDATE") != 0)
					{
						return INV_EVENT;
					}
					

					int i = 0;
					for(i = 0; i < 18; i++)
					{
						if(eventPropOpt[i] > 1 || eventPropOpt[15] > 0)
						{
							return INV_EVENT;
						} 
					}
					if((eventPropOpt[16] == 1 && eventPropOpt[17] == 1)
					|| strcmp(p1->propDescr,"") == 0
					|| !p1->propDescr)
					{
						return INV_EVENT;
					}
					test2 = test2->next;
				}
			}
			
			if(!e1->alarms)
			{
				return INV_EVENT;
			}
			if(e1->alarms)
			{
				Node *test3 = e1->alarms->head;
				Alarm *a1;
				while(test3 != NULL)
				{
					a1 = test3->data;
					if(!a1->trigger)
					{
						return INV_ALARM;
					}
					if(strcmp(a1->action,"") == 0 || strcmp(a1->trigger,"") == 0)
					{
						return INV_ALARM;
					}
					
					if(!a1->properties)
					{
						return INV_ALARM;
					}
					if(a1->properties)
					{
						int eventAlm[3];
						eventAlm[0] = 0; //DURATION
						eventAlm[1] = 0; //REPEAT
						eventAlm[2] = 0; //ATTACH
						Node *test4 = a1->properties->head;
						Property *p2;
						while(test4 != NULL)
						{
							p2 = test4->data;
							
							if(strcmp(p2->propName,"DURATION") == 0)
								eventAlm[0]++;
							else if(strcmp(p2->propName,"REPEAT") == 0)
								eventAlm[1]++;
							else if(strcmp(p2->propName,"ATTACH") == 0)
								eventAlm[2]++;
							else
								return INV_ALARM;	

							if(eventAlm[2] > 1 || eventAlm[1] > 1 || eventAlm[0] > 1 ||
							strcmp(p2->propDescr,"") == 0)
							{
								return INV_ALARM;
							}
								
							test4 = test4->next;
						}
						if((eventAlm[0] == 1 && eventAlm[1] == 0) || (eventAlm[0] == 0 && eventAlm[1] == 1)) 
						{
							return INV_ALARM;
						}
					}
					test3 = test3->next;
				}	
			}
			test1 = test1->next;	
		}
	}
	return OK;
}

/** Function to converting a DateTime into a JSON string
 *@pre N/A
 *@post DateTime has not been modified in any way
 *@return A string in JSON format
 *@param prop - a DateTime struct
 **/
char* dtToJSON(DateTime prop)
{
	char *returning;
	
	if(prop.UTC == 0)
	{
		returning = (char*)malloc(sizeof(char) * (strlen("{\"date\":\"") +
		strlen(prop.date) + strlen("\",\"time\":\"") +
		strlen(prop.time) + strlen("\",\"isUTC\":false}")+2));
		sprintf(returning,"{\"date\":\"%s\",\"time\":\"%s\",\"isUTC\":false}",prop.date,prop.time);
	}
	else if(prop.UTC == 1)
	{
		returning = (char*)malloc(sizeof(char) * (strlen("{\"date\":\"") +
		strlen(prop.date) + strlen("\",\"time\":\"") +
		strlen(prop.time) + strlen("\",\"isUTC\":true}")+2));
		sprintf(returning,"{\"date\":\"%s\",\"time\":\"%s\",\"isUTC\":true}",prop.date,prop.time);
	}
	return returning;
}

/** Function to converting an Event into a JSON string
 *@pre Event is not NULL
 *@post Event has not been modified in any way
 *@return A string in JSON format
 *@param event - a pointer to an Event struct
 **/
/*char* eventToJSON(const Event* event)
{
	if(event == NULL)
	{
		return "{}";
	}
	
	char *startDT;
	char *returning;
	char *sumVal = "";
	int propVal,almVal;
	
	propVal = 3;
	almVal = 0;
	
	if(event->properties)
	{
		Node *test2 = event->properties->head;
		Property *p1;
		while(test2 != NULL)
		{
			p1 = test2->data;
			if(strcmp(p1->propName,"SUMMARY") == 0)
			{
				sumVal = (char*)malloc(sizeof(char) * 
				(strlen(p1->propDescr)+1));
				strcpy(sumVal,p1->propDescr);
			}
			propVal++;			
			test2 = test2->next;
		}
		
	}

	if(event->alarms)
	{
		Node *test3 = event->alarms->head;
		while(test3 != NULL)
		{
			almVal++;
			test3 = test3->next;
		}
	}
	
	startDT = dtToJSON(event->startDateTime);
	returning = (char*)malloc(sizeof(char) * (strlen("{\"startDT\":") +
	strlen(startDT) + strlen(",\"numProps\":") +
	sizeof(int) + strlen(",\"numAlarms\":") +
	sizeof(int) + strlen(",\"summary\":\"") +
	strlen(sumVal) +  strlen("\"}")+2));
	sprintf(returning,"{\"startDT\":%s,\"numProps\":%d,\"numAlarms\":%d,\"summary\":\"%s\"}",
	startDT,propVal,almVal,sumVal);

	if(strlen(sumVal) > 0)
		free((char*)sumVal);
	if(strlen(startDT) > 0)
		free((char*)startDT);
	return returning;
}*/

/** Function to converting an Event list into a JSON string
 *@pre Event list is not NULL
 *@post Event list has not been modified in any way
 *@return A string in JSON format
 *@param eventList - a pointer to an Event list
 **/
char* eventListToJSON(const List* eventList)
{
	if(!eventList || eventList == NULL)
	{
		return "[]";
	}
	char *returning;
	
	if(eventList)
	{
		Node *events = eventList->head;
		Event *e1;
		char *info;
		int length = 0;
		returning = (char*)malloc(sizeof(char) * (strlen("[") +1));
		sprintf(returning,"[");
		length = strlen(returning);
		while(events != NULL)
		{
			e1 = events->data;
			info = eventToJSON(e1);
			returning = (char*)realloc(returning, sizeof(char) * (length + strlen(info) + 1));
			strcat(returning,info);
			if(events->next != NULL)
			{
				returning = (char*)realloc(returning, sizeof(char) * (length + strlen(info) + 4));
				strcat(returning,",");				
			}
			length = strlen(returning);
			events = events->next;
			free((char*)info);
		}
		returning = (char*)realloc(returning,sizeof(char) * (strlen(returning) + 2));
		strcat(returning,"]");
	}
	printf("EVENT LIST: %s\n",returning);
	size_t test = strlen(returning);
	printf("EVENT LENGTH: %zd\n",test);
	for(int i = 0; i < 130; i++)
	{
		printf("\"%c\"",returning[i]);
	}
	return returning;
}

/** Function to converting a Calendar into a JSON string
 *@pre Calendar is not NULL
 *@post Calendar has not been modified in any way
 *@return A string in JSON format
 *@param cal - a pointer to a Calendar struct
 **/
 
char* calendarToJSON(const Calendar* cal)
{
	if(cal == NULL)
		return "{}";
		
	char *returning;
	int propVal,eventVal,iterVal;
	
	iterVal = 0;
	propVal = 2;
	eventVal = 1;
	
	if(cal->properties)
	{
		Node *test = cal->properties->head;
		while(test != NULL)
		{
			propVal++;
			test = test->next;
		}
	}
		
	if(cal->events)
	{
		Node *test1 = cal->events->head;	 		
		while(test1 != NULL)
		{
			if(iterVal >= 1)
				eventVal++;
				
			iterVal++;
			test1 = test1->next;
		}
	}
	returning = (char*)malloc(sizeof(char) * (strlen("{\"version\":") +
	sizeof(float) + strlen(",\"prodID\":\"") +
	strlen(cal->prodID) + strlen("\",\"numProps\":") +
	sizeof(int) + strlen(",\"numEvents\":") + 
	sizeof(int) + strlen("}") +2));
	sprintf(returning,"{\"version\":%d,\"prodID\":\"%s\",\"numProps\":%d,\"numEvents\":%d}",(int)cal->version,
	cal->prodID,propVal,eventVal);
	printf("%s\n",returning);
	return returning;
}

/** Function to converting a JSON string into a Calendar struct
 *@pre JSON string is not NULL
 *@post String has not been modified in any way
 *@return A newly allocated and partially initialized Calendar struct
 *@param str - a pointer to a string
 **/
Calendar* JSONtoCalendar(const char* str)
{
	if(str == NULL)
		return NULL;

	char *str1 = malloc(sizeof(char) * (strlen(str) + 1));
	char *str2;
	strcpy(str1,str);
	char *token;
	int vFlag,proFlag;
	vFlag = 0;
	proFlag = 0;
	
	Calendar *cal1 = (Calendar*)malloc(sizeof(Calendar));	
	cal1->version = 0.0;
	strcpy(cal1->prodID,"");
	cal1->events = initializeList(&printEvent,deleteEvent,&compareEvents);	
	cal1->properties = initializeList(&printProperty,deleteProperty,&compareProperties);	

	token = strtok(str1,",");
	while(token != NULL)
	{
		char *token1;
		str2 = malloc(sizeof(char) *(strlen(token) + 1));
		strcpy(str2,token);	
		token = strtok(NULL,",");		
		token1 = strtok(str2,":");
		if(strstr(token1,"version") != NULL)
		{
			vFlag = 1;
		}				
		else if(strstr(token1,"prodID") != NULL)
		{
			proFlag = 1;
		}				

		token1 = strtok(NULL,"\"}");
		if(vFlag == 1)
		{
			cal1->version = atof(token1);
			vFlag = 0;
		}				
		else if(proFlag == 1)
		{
			strcpy(cal1->prodID,token1);
			proFlag = 0;
		}
		 if(token != NULL)
			free(str2);
	}
	if(str1)
		free(str1);
	if(token == NULL && str2)
		free(str2);
	return cal1;
}

/** Function to converting a JSON string into an Event struct
 *@pre JSON string is not NULL
 *@post String has not been modified in any way
 *@return A newly allocated and partially initialized Event struct
 *@param str - a pointer to a string
 **/
Event* JSONtoEvent(const char* str)
{
	if(str == NULL)
		return NULL;
	char *str1 = malloc(sizeof(char) * (strlen(str) + 1));
	char *str2;
	strcpy(str1,str);
	char *token;
	int uFlag;
	Event *e1 = (Event*)malloc(sizeof(Event));	
	e1->properties = initializeList(&printProperty,deleteProperty,&compareProperties);
	e1->alarms = initializeList(&printAlarm,deleteAlarm,&compareAlarms);			

	token = strtok(str1,",");
	while(token != NULL)
	{
		char *token1;
		str2 = malloc(sizeof(char) *(strlen(token) + 1));
		strcpy(str2,token);	
		token = strtok(NULL,",");		
		token1 = strtok(str2,":");
		if(strstr(token1,"UID") != NULL)
		{
			uFlag = 1;
		}				
		token1 = strtok(NULL,"\"}");
		if(uFlag == 1)
		{
			strcpy(e1->UID,token1);
			uFlag = 0;
		}				
		if(str2)
			free(str2);
	}
	
	if(str1)
		free(str1);
	return e1;
}

/** Function to adding an Event struct to an ixisting Calendar struct
 *@pre arguments are not NULL
 *@post The new event has been added to the calendar's events list
 *@return N/A
 *@param cal - a Calendar struct
 *@param toBeAdded - an Event struct
 **/
void addEvent(Calendar* cal, Event* toBeAdded)
{
	if(cal == NULL || toBeAdded == NULL)
	{
		return;
	}
	if(cal->events)
	{
		insertBack(cal->events,toBeAdded);
		
	}
}

void deleteEvent(void* toBeDeleted)
{
	return;
}

int compareEvents(const void* first, const void* second)
{		
	return 1;
}

char* printEvent(void* toBePrinted)
{
	if(!toBePrinted)
	{
		return NULL;
	}
	char *returning;
	Event* e1;
	e1 = (Event*)toBePrinted;

	returning = (char*)malloc(sizeof(char) * (strlen("UID: ") +
	strlen(e1->UID) + strlen("\nCREATIONDATE: ") + 
	strlen(e1->creationDateTime.date) + strlen("\nCREATIONTIME: ") +
	strlen(e1->creationDateTime.time) + strlen("\nSTARTDATE: ") +
	strlen(e1->startDateTime.date) + strlen("\nSTARTTIME: ") +
	strlen(e1->startDateTime.time) + 2));
 	sprintf(returning,"UID: %s\nCREATIONDATE: %s\nCREATIONTIME: %s\nSTARTDATE: %s\nSTARTTIME: %s\n"
 	,e1->UID,e1->creationDateTime.date,e1->creationDateTime.time,e1->startDateTime.date
 	,e1->startDateTime.time);
 			
	return returning;
}

void deleteAlarm(void* toBeDeleted)
{
	return;
}

int compareAlarms(const void* first, const void* second)
{
	return 1;
}

char* printAlarm(void* toBePrinted)
{
	if(!toBePrinted)
	{
		return NULL;
	}
	Alarm *a1;
	char *returning;
	a1 = (Alarm*)toBePrinted;
	returning = (char*)malloc(sizeof(char) * (strlen("ACTION: ") +
	strlen(a1->action) + strlen("\nTRIGGER: ") + 
	strlen(a1->trigger) + 2));
 	sprintf(returning,"ACTION: %s\nTRIGGER: %s\n",a1->action,a1->trigger);		
	
	return returning;
}

void deleteProperty(void* toBeDeleted)
{
	return;
}

int compareProperties(const void* first, const void* second)
{
	return 1;	
}

char* printProperty(void* toBePrinted)
{
	if(!toBePrinted)
	{
		return NULL;
	}
	Property *p1;
	char *returning;
	p1 = (Property*)toBePrinted;
	
	returning = (char*)malloc(sizeof(char) * (strlen("PROPNAME: ") +
	strlen(p1->propName) + strlen("\nPROPDESC: ") + 
	strlen(p1->propDescr) + 2));
 	sprintf(returning,"PROPNAME: %s\nPROPDESC: %s\n",p1->propName,p1->propDescr);		
	return returning;
}

void deleteDate(void* toBeDeleted)
{
	return;
}

int compareDates(const void* first, const void* second)
{	
	return 1;
}

char* printDate(void* toBePrinted)
{
	if(!toBePrinted)
	{
		return NULL;
	}
	char *returning;
	returning = "HELLO\n";
	return returning;
}
