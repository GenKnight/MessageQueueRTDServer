# Message Queue RTD Server
Messaging Queue RTD Server (.dll) intended to provide Real-Time Data in Excel

[RTD] function in Excel retrieves real-time data from a program that supports COM automation.
[Messaging Queue] is used for inter-process communication (IPC). They use a queue for messaging content.


### Installation
- Download the appropriate 32 or 64-bit .dll for your system.  Note:  The appropriate .dll for your system must match the architechure of Excel installed on your system.  You can find the architechure of your Excel intallation from the popup dialog within Excel from File > Account  > About Excel.  You may have 32-bit Excel installed even on a 64-bit operating system in which case you should install the 32-bit version of this .dll
- Use [RegSvr32] to properly register your .dll on your system
- Within Excel, goto File > Options > Add-Ins and enable 'MessageQueue.RTDServer'


### How it Works
Within Excel, enter the formula '=RTD("MessageQueue.RTDServer",,"MsgQueueName").  This will open a Messaging Queue on your system named "MsgQueueName" and will display any message sent to that Queue on your system.  Messages can be sent to the queue on your system from other processes or applications on your system such as Python.  As an example, this can be done in Python leveraging [ctypes] to load the 'MessageQueueRTDServer.dll' into a process memory and using the API 'MQSend(queue_name, msg)'.  Here you can substitute 'queue_name' with 'MsgQueueName' that was specified in the RTD function and any message sent through the API will appear in Excel. You can specify multiple RTD functions within Excel with different Queue Names to have different content messages sent to Excel. 


### Development Setup
This project has been built using Visual Studio 2015 C++ and can output both 32 and 64-bit dlls that interact with Excel.


### License
Licensed under the Apache License, Version 2.0 (the "License"). You may obtain a copy of the License at
  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the License for the specific language governing permissions and limitations under the License.


[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [RTD]: <https://support.office.com/en-us/article/RTD-function-e0cc001a-56f0-470a-9b19-9455dc0eb593>
   [Messaging Queue]: <https://en.wikipedia.org/wiki/Message_queue>
   [RegSvr32]: <https://support.microsoft.com/en-us/kb/249873>
   [ctypes]: <https://docs.python.org/2/library/ctypes.html>
