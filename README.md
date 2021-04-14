# UnrealEngine Network Failure Bug Demonstration
A demonstration for UE network failure while replicating arrays.

## What kinds of failure?
In some cases, UE will fail to replicate arrays due to a design flaw which resulting in out-of-sync between the server and clients.  
## Steps to Reproduce
1. Get any UE4 version that above 4.18 (Haven't tried in older version but they should fail as well.)
2. Clone/Download this project, compile and open with your engine.
3. Project should load the default map for you, if not, you need to load "NewMap".
4. Make sure you've in Play As Client mode (In older engine version,make sure you've checked Run Dedicated Server ).
![](https://github.com/NexusEast/UnrealEngineNetworkFailureBug/blob/master/NetworkFailureBug/img/ds.gif)
![](https://github.com/NexusEast/UnrealEngineNetworkFailureBug/blob/master/NetworkFailureBug/img/2.jpg)
5. Open up Output Log window.(For observe client replication notify log).
6. Set server outgoing packet loss 80% (For older versions of engine that do not have settings for network emulation, you need to execute command "Pkt Loss 80" at server side every time you start PIE session.)
![](https://github.com/NexusEast/UnrealEngineNetworkFailureBug/blob/master/NetworkFailureBug/img/pktls.jpg)
7. Type "OnRep_RootProp" into your Output Log Filter.(So that you can clearly observe whether the client has received the notification.)
8. Now start your PIE session and make sure you've connected to your session server.
9. Hit "~" to show console, type "ce ReadyUp" then hit Enter. Observe Output Log window, a "OnRep_RootProp" will appear in seconds. This means the client has successfully received replication notify.
10. Once confirmed the client replication notify in step 9, now open up console again and type "ce TriggerBug" then hit Enter.
11. Observe Output Log, there is a high chance that the client won’t receive notify. Wait for 10 or more seconds to confirm. But if you do receive the notify, then retry from step 9.
12. After confirming the client did not receive replication notify, you can now put a breakpoint at line 23 in TestRepComp.cpp, to see the replicated property which should be different between both sides.
13. Now, this property is officially out-of-sync.
## Why?
I'll describe this issue briefely.  
Unreal Engine has mechanisms to ensure the client receives the latest replicated property values.  
When packet loss occurs (no ACK is received), when the mechanism sends the current latest value, it will compare it with the value that has been compared last time, and generate a Change Command Array.
The problem lies in the process of property comparison: 
Here's a diagram to help us understand what happened.  
![](https://github.com/NexusEast/UnrealEngineNetworkFailureBug/blob/master/NetworkFailureBug/img/dsbug.jpg)
To fix this, the mechanisms might need to modify heavily and I am very curious how Epic Dev will fix this problem.
