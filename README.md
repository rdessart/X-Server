# X-Server
Network Available API for simulator interaction.

## Implemented Functionalities

1. Speak
2. Set Dataref
3. Get Dataref
4. Register Dataref (create name - dataref link)
5. Set Registered Dataref
6. Get Registered Dataref
7. Get Dataref Information (Readonly, Type, etc.)
8. Get registered Dataref Information
9. LoadDLL call to load DLL of functionality
10. Realease DLL to detach DLL
11. Create a Getter FlightLoop (a Callback with subscribed datarefs are returned every X Seconds or Frame)
12. Subscribe dataref (Link a register dataref with a callback)
13. UnSubscride dataref
14. Unregister callback

## Gneral Data fromat

### Dataref
Json Subtype 

| Field | Type | Desc |
| --- | --- | --- |
| Link | String | The path to the dataref |
| ConvertionFactor | Double | A convertion factor to be applied to the dataref |
| Type | String | The target type of the dataref (int, float, double, data, vector) |
| Value | X | The value to be set/get of the dataref. This will be casted into the type of dataref.


#### Values

| Type | Format |
| --- | --- |
| INT | 1 |
| FLOAT/DOUBLE | 1.0 |
| FLOAT ARRAY | {1.0, 2.0, 3.0} |
| INT ARRAY | {1, 2, 3} |
| DATA | "value to be set" |


#### Conversion Factor
GET: Value * ConvertionFactor = Presented Value

SET: Value / ConvertionFactor = Value written


## Operations

### 1.Speak
Get X-Plane text to speach engine to say the phrase.

|Parameter (Json) | Optional | Description |
|---|--|------|
| Text | N | The text to be spoken |

### 2.Set Dataref
Set a value to a dataref

|Parameter (Json) | Optional | Description |
|---|--|------|
| Dataref | N | The target dataref |

## Planned Feature with this branch 

- [ ] Aquired Aircrafts
- [ ] Release Aircrafts
- [ ] Set Aircrafts Count
