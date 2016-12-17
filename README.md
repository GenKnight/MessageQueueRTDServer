# Message Queue RTD Server
A highly efficient Message Queue RTD Server (.dll) that is intended to provide Real-Time Data in Excel

[RTD] function in Excel retrieves real-time data from a program that supports COM automation.
[Messaging Queue] is used for inter-process communication (IPC).


### Installation
- Download the appropriate 32 or 64-bit .dll for your system.  Note:  The appropriate .dll for your system must match the architechure of Excel installed on your system.  You can find the architechure of your Excel intallation from the popup dialog within Excel from File > Account > About Excel.  You may even have 32-bit Excel installed on a 64-bit operating system in which case you should install the 32-bit version of this .dll
- Use [RegSvr32] to properly register your .dll on your system
- Within Excel, goto File > Options > Add-Ins and enable 'MessageQueue.RTDServer'


### How it Works
Within Excel, enter the formula '=RTD("MessageQueue.RTDServer",,"MsgQueueName")'.  You can substitute "MsgQueueName" with a name your choice.  This will open a Messaging Queue on your system named "MsgQueueName" and will display any message sent to that queue on your system.  Messages can be sent from other processes or applications on your system, such as Python.  For example, you can use Python's [ctypes] to load the 'MessageQueueRTDServer.dll' into memory and use the exposed API to interact with the queues.  For example 'MQSend("MsgQueueName", "msg")' will send "msg" to the matching RTD function(s) in Excel.  You can specify multiple RTD functions within Excel with different queue Names to have different content messages sent. 


### Exposed DLL API
- HRESULT MQOpen(const char *qname)
- HRESULT MQClose(const char *qname)
- HRESULT MQSend(const char *qname, const char \*msg)
- HRESULT MQRecv(const char *qname, char \*\*msg)
Note:  The qname and sent msg are restricted to 15 characters for efficiency reasons.  Longer names will be capped to the first 15 characters


### Development Setup
This project has been built using Visual Studio 2015 C++ and can output both 32 and 64-bit dlls that interact with Excel.


### Technologies Used
This project uses the technologies listed below
- AltDLL
- IDispatch
- IRTDServer
- IRTDUpdateEvent
- VARIANT
- SAFEARRAY
- BSTR
- Boost
- Interprocess Communication
- Interprocess Mutex
- Interprocess Condition
- Shared Memory Object
- Message Queue


### License
Licensed under the Apache License, Version 2.0 (the "License"). You may obtain a copy of the License at
  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the License for the specific language governing permissions and limitations under the License.


[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [RTD]: <https://support.office.com/en-us/article/RTD-function-e0cc001a-56f0-470a-9b19-9455dc0eb593>
   [Messaging Queue]: <https://en.wikipedia.org/wiki/Message_queue>
   [RegSvr32]: <https://support.microsoft.com/en-us/kb/249873>
   [ctypes]: <https://docs.python.org/2/library/ctypes.html>
