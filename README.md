CSE 687 Object Oriented Design


Project Phase 1 - Map Reduce WorkFlow:
	The Map_Reduce Project is a standalone command-line program written in Visual Studio using the C++ Language.
The command-line program runs a word count MapReduce workflow. This program runs as a single process, with no threads or multiple processes.
	The MapReduceNativeTest is a Native Test Project that tests the Map_Reduce command line program.



Project Phase 2 - Map Reduce WorkFlow with DLL
	Update 5/9/23: This project has been updated to take in a Map DLL and Reduce DLL from the user rather than using the source code's cpp and h files
	
	
Project Phase 3 - Map Reduce Workflow with Map Thread and Reducer Thread (while maintaining DLL References)
	Update 5/31/23: 1) Upload Phase 3 submission which incorporates parallel threading. Map Processes and Reduce Processes were included and also reference to the Phase 2 DLLs.
					2) Reformat folder structure and include Phase 1 solution and Phase 2 solution files for history
					
Project Phase 4 - Map Reduce Workflow with a Client Stub Socket and a Server Controller Socket
	Updated 6/14/23: 1) Upload Phase 4 submission which incorporates creating a socket connection between the controller (workflow) and the stub process (StubProcess)
	
-------------------------------------------------------------------------------
TO RUN THE PROGRAM:
	- Option 1: Compile the *.cpp and *.h files using the command line.  


	- Option 2: In Visual Studio, load the Map_Reduce.sln file from the repository. 
				Make sure to compile the DLL files separately. Map.lib solution files are in the Map folder and Reduce.lib files are in the Reduce folder.


TO RUN THE TEST:
	- In Visual Studio, load the *.sln file from the repository. 
	- Set the startup project as MapReduceNativeTest.
	- Navigate to Test on the Visual Studio Menu Bar -> then Run All Tests


-------------------------------------------------------------------------------
GITHUB AUTHORS:
@jasminekp
@risktakingkev
@betito44