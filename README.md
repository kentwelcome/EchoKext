##EchoKext
Simple tool to communicate between kernel mode kext and user mode daemon on Mac OSX

##How to Run
TBD

##TODO
- [x] Implement Mac OS X general kext
	- [x] Use sysctl to trigger kext send msg to User mode
	- [x] Implement mechanism to communicate between kernel and user space
- [ ] Implement Mac OS X IOKit kext
	- [ ] Use sysctl to trigger IOKit kext send msg to User mode
	- [ ] Implement mechanism to communicate between kernel and user space 
- [ ] Implement Mac OS X command line daemon tool
    - [x] Create socket and connection with kext
    - [x] Send command to kext
	- [ ] Listen the notification send from kext or IOKit kext