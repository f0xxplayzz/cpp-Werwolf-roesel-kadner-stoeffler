Structure of a Client Message:
0:id
1:role
2:phase
3:DONE/NOTDONE
4:Case
5:ID of target
6-16:string name

Structure of Server Messages:
0:phase
1:length_of_vector
2...2+lengthOfVector:IDs of possibilities
...:String of possibilities

http://underthepressure.net/how-to-sleep-in-c/