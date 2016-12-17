# Message Queue RTD Server for Excel
A highly efficient Message Queue RTD Server (.dll) that is intended to provide Real-Time Data in Excel.

[RTD] function in Excel retrieves real-time data from a program that supports COM automation.

[Messaging Queue] is used for inter-process communication (IPC).

[Shared Memory Objects] are used to efficiently send inter-process messages and notifications.

### How it Works
Within Excel, enter the formula `=RTD("MessageQueue.RTDServer",,"MsgQueueName")`.  You can substitute *"MsgQueueName"* with a name of your choice.  This will open a Messaging Queue on your system named *"MsgQueueName"* and will display any message sent to that queue on your system.

Messages can be sent from other processes or applications on your system, such as Python.  For example, you can use Python's [ctypes] to load the 'MessageQueueRTDServer.dll' into memory and use the exposed API to interact with the queues.  This can be done as follows:

`MQSend("MsgQueueName", "Hello Excel")`

The above will send *"Hello Excel"* to the matching RTD function(s) with the *"MsgQueueName"* name in Excel.  You can specify multiple RTD functions within Excel with different queue names to have different messages sent. All messages are sent as strings and you can cast strings to numbers or other using Excel functions as needed.

Additional topic names can be supplied to RTD funtions in Excel that can interact with this dll.  For example  `=RTD("MessageQueue.RTDServer",,"Topic1","Topic2")` will open a Message Queue on your system with the name *"Topic1_Topic2"*.  Notice that *'_'* is inserted into the Message Queue name opened on the system between topics.  You can send a message to this queue from another application with the following API call `MQSend("Topic1_Topic2", "Hello Excel")`.

Addionally, you can create the following function in Excel
`=RTD("MessageQueue.RTDServer",,"Topic1","Topic2","Topic3")` which will open the Message Queue name *"Topic1_Topic2_T"* on your system.  Notice here that the queue name only takes the *'T'* from *'Topic3'*.  This is because Message Queue names opened on the system are capped at 15 characters.  You can send a message to this queue from another application with the following API call `MQSend("Topic1_Topic2_T", "Hello Excel")`.  The other restriction is that messages sent from other applications are also capped at 15 characters.  So sending the message *"Hello Excel"* will be received in full in Excel but the message *"Hello Excel from the otherside"* will yield *"Hello Excel fro"*.

Finally, it should be mentioned that the implementation of this RTD server leverages the inherent efficienies of RTD functions within Excel.  No polling or timers are used to send or receive messages.  This means that new messages sent to a message queue from another application will only be updated in Excel at the time the message is sent.  This keeps Excel responsive even when there are many RTD message queues opened on the system.  If many message queues are updated at the same time, *SAFEARRAYS* efficiently update all the values within Excel at once.  Also, when Excel shuts down, all associated message queues from RTD functions will be closed on the system and re-open the next time Excel loads the RTD functions.


### Exposed DLL API
- HRESULT MQOpen(const char \*qname)
- HRESULT MQClose(const char \*qname)
- HRESULT MQSend(const char \*qname, const char \*msg)
- HRESULT MQRecv(const char \*qname, char \*\*msg)

Note:  The qname and sent msg are restricted to 15 characters for efficiency reasons.  Longer names will be capped to the first 15 characters


### Installation
- Download the appropriate 32 or 64-bit .dll for your system.  Note:  The appropriate .dll for your system must match the architechure of Excel installed on your system.  You can find the architechure of your Excel intallation from the popup dialog within Excel from `File > Account > About Excel`.  You may even have 32-bit Excel installed on a 64-bit operating system in which case you should install the 32-bit version of this .dll
- Use [RegSvr32] to properly register your .dll on your system
- Within Excel, goto `File > Options > Add-Ins` and enable *MessageQueue.RTDServer*


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
Licensed under the Apache License, Version 2.0 (the "License"). You may obtain a copy of the License at:

  [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the License for the specific language governing permissions and limitations under the License.


[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [RTD]: <https://support.office.com/en-us/article/RTD-function-e0cc001a-56f0-470a-9b19-9455dc0eb593>
   [Messaging Queue]: <https://en.wikipedia.org/wiki/Message_queue>
   [Shared Memory Objects]: <https://en.wikipedia.org/wiki/Shared_memory>
   [RegSvr32]: <https://support.microsoft.com/en-us/kb/249873>
   [ctypes]: <https://docs.python.org/2/library/ctypes.html>
