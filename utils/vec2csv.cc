#include <map>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <regex.h>
#include <unistd.h>

using namespace std;

// Data Model:

// keep track of the column name for object 
map< string , unsigned int > colByObjVec;
vector< string > objVecByCol;
map< unsigned int, unsigned int> colByVecIdx;
typedef pair< unsigned int, string > idxValPair;
multimap< double, idxValPair > idxValPairByTime;

double start = 0.0;
double end = -1.0;
double delta = -1.0;
bool statMode = false;
bool noSample = false;

int
parseVectorNamesFile(const char *fileName) 
{
  ifstream f(fileName);
  if (!f.good()) {
	 cerr << "-E- Failed to open file: " << fileName << endl;
	 return(1);
  }
  char buff[1024];
  while (f.good()) {
	 f.getline(buff, 1024);
	 if (strlen(buff)) {
		// cerr << "-V- object:vector: " << buff << endl;
		colByObjVec[buff] = objVecByCol.size();
		objVecByCol.push_back(string(buff));
	 }
  }
  f.close();

  cerr << "-I- Looking for " << colByObjVec .size()
		 << " object:vector pairs defined in: " << fileName << endl;
  return(0);
}

#define FIELD(t, f, n) while (1) { \
   strncpy(t, f + matches[n].rm_so, matches[n].rm_eo - matches[n].rm_so + 1); \
	t[matches[n].rm_eo - matches[n].rm_so] = '\0'; \
   break;};

int
extractVectorNumbersFromVectorFile(const char *fileName) {
  regex_t re;
  int status;
  regmatch_t matches[4];
  
  status = 
	 regcomp(&re, "^vector\\s+([0-9]+)\\s+(\\S+)\\s+(\\S.*\\S)\\s+\\S+$",
				REG_EXTENDED);
  if (status) {
	 cerr << "-E- BUG IN VECTOR REGEXP" << endl;
	 exit(3);
  }

  ifstream f(fileName);
  if (!f.good()) {
	 cerr << "-E- Failed to open file: " << fileName << endl;
	 return(1);
  }
  char buff[1024];
  while (f.good()) {
	 f.getline(buff, 1024);
	 if (!regexec(&re, buff, 4, matches, 0)) {
		char nStr[16], oStr[256], vStr[512];
		FIELD(nStr, buff, 1);
		FIELD(oStr, buff, 2);
		FIELD(vStr, buff, 3);
		// cerr << "-V- Found n: " << nStr << " o: " << oStr << " v: " << vStr << endl;
		string objVec = string(oStr)+
		  string(":")+string(vStr);
		if (colByObjVec.find(objVec) != colByObjVec.end()) {
		  unsigned int vecIdx = atoi(nStr);
		  colByVecIdx[vecIdx] = colByObjVec[objVec];
		}
	 }
  }
  f.close();
  cerr << "-I- Found " << colByVecIdx.size() << " object:vector pairs" 
		 << " in: " << fileName << endl;
  return(0);
}

int 
extractAndSortVectorFileData(const char *fileName) 
{
  regex_t re;
  int status;
  regmatch_t matches[4];
  
  status = 
	 regcomp(&re, "^([0-9]+)\\s+[0-9]+\\s+(\\S+)\\s+(\\S+)$",
				REG_EXTENDED);
  if (status) {
	 cerr << "-E- BUG IN DATA REGEXP" << endl;
	 exit(3);
  }

  ifstream f(fileName);
  if (!f.good()) {
	 cerr << "-E- Failed to open file: " << fileName << endl;
	 return(1);
  }
  char buff[1024];
  while (f.good()) {
	 f.getline(buff, 1024);
	 if (!regexec(&re, buff, 4, matches, 0)) {
		char nStr[16], tStr[256], dStr[512];
		FIELD(nStr, buff, 1);
		FIELD(tStr, buff, 2);
		FIELD(dStr, buff, 3);
		
		// only care about some idxs
		unsigned int vecIdx = atoi(nStr);
		if (colByVecIdx.find(vecIdx) == colByVecIdx.end()) continue;
		double t = atof(tStr);
		idxValPair p(colByVecIdx[vecIdx], string(dStr));
		idxValPairByTime.insert(pair<double,idxValPair>(t,p));
	 }
  }
  f.close();
  cerr << "-I- Found " << idxValPairByTime.size() 
		 << " relevant data values in: " << fileName << endl;
  return(0);
}

int formatCsv()
{
  vector< string > curVal(objVecByCol.size(), string(""));
  double prevT = -1.0;
  double prevDump = 0.0;
  // create headers
  if (statMode) {
	 cout << "T,AVG,MIN,MAX" << endl;
  } else {
	 cout << "T";
	 for (unsigned i = 0; i < objVecByCol.size(); i++) {
		cout << "," << objVecByCol[i];
	 }
	 cout << endl;
  }

  // go over all data
  multimap< double, idxValPair >::const_iterator I;
  double t;
  unsigned numTs = 0;
  for (I = idxValPairByTime.begin(); I != idxValPairByTime.end(); I++) {
	 t = (*I).first;
	 if ((t != prevT) && (prevT > 0)) {
		if (t - prevDump > delta) {
		  prevDump = t;
		  if (statMode) {
			 double minV, maxV, sumV = 0, V;
			 minV = maxV = atof(curVal[0].c_str());
			 for (unsigned i = 0; i < curVal.size(); i++) {
				V = atof(curVal[i].c_str());
				if (V < minV) minV = V;
				if (V > maxV) maxV = V;
				sumV += V;
			 }
			 cout << t << "," << sumV/curVal.size() << "," << minV << ","
					<< maxV << endl;
		  } else {
			 cout << t;
			 for (unsigned i = 0; i < curVal.size(); i++) {
				cout << "," << curVal[i];
			 }
			 cout << endl;
		  }
		}
		numTs++;
	 }
	 if (noSample) for (unsigned i = 0; i < curVal.size(); i++) curVal[i] = "";
	 prevT = t;
	 curVal[(*I).second.first] = (*I).second.second;
  }
  if (statMode) {
	 double minV, maxV, sumV = 0, V;
	 minV = maxV = atof(curVal[0].c_str());
	 for (unsigned i = 0; i < curVal.size(); i++) {
		V = atof(curVal[i].c_str());
		if (V < minV) minV = V;
		if (V > maxV) maxV = V;
		sumV += V;
	 }
	 cout << t << "," << sumV/curVal.size() << "," << minV << ","
			<< maxV << endl;
  } else {
	 cout << t;
	 for (unsigned i = 0; i < curVal.size(); i++) {
		cout << "," << curVal[i];
	 }
	 cout << endl;
  }
  numTs++;
  cerr << "-I- Output " << numTs << " different time values" << endl;
  return(0);
}

int 
main(int argc, char** argv) 
{
  string vecFileName;
  string namesFileName;
  // bool verbose;
  extern char *optarg;
  extern int optopt;
  char c;
  int errflg = 0;

  const char *desc = 
	 "OMNET Vector to CSV Converter\n\n"
	 "Extracts set of vectors from Omnet vector file into CSV format\n" 
	 "\nDescripiton:\n" 
	 " creates a CSV file with columns for each vector included in\n"
	 " the vectors file which is of the format: object:vector\n"
	 "ARGUMENTS\n\n"
	 "-i vectors-file : the vector file generated by omnet\n"
	 "-n names-file : a file holding OBJ-NAME:VEC-NAME on each line\n"
	 "\nOPTIONS:\n\n"
	 "-s start-time : report starting at the given time [sec]\n"
	 "-e end-time   : end the report at the given time [sec]\n"
	 "-d delta-time : report only every delta-time [sec]\n"
	 "-q            : only report AVG, MIN, MAX of each time values\n"
	 "-k            : do not keep previous value (no hold - produce empty fields)\n";
  
  while ((c = getopt(argc, argv, "i:n:s:e:d:qk")) != -1) {
	 switch(c) {
	 case 'i':
		vecFileName = string(optarg);
		break;
	 case 'n':
		namesFileName = string(optarg);
		break;
	 case 's':
		start = atof(optarg);
		break;
	 case 'e':
		end = atof(optarg);
		break;
	 case 'd':
		delta = atof(optarg);
		break;
	 case 'q':
		statMode = true;
		break;
	 case 'k':
		noSample = true;
		break;
	 case ':':       /* -f or -o without operand */
		fprintf(stderr,
				  "Option -%c requires an operand\n", optopt);
		errflg++;
		break;
	 case '?':
		fprintf(stderr,
				  "Unrecognized option: -%c\n", optopt);
		errflg++;
	 }
  }

  if (vecFileName.size() == 0) {
	 cerr << "Missing arg: -i" << endl;
	 errflg++;
  }
  if (namesFileName.size() == 0) {
	 cerr << "Missing arg: -n" << endl;
	 errflg++;
  }

  if (errflg) {
    fprintf(stderr, "%s", desc);
    exit(2);
  }


  if (parseVectorNamesFile(namesFileName.c_str())) {
	 exit(1);
  }
  
  if (extractVectorNumbersFromVectorFile(vecFileName.c_str())) {
	 exit(1);
  }
  
  if (extractAndSortVectorFileData(vecFileName.c_str())) {
	 exit(1);
  }
  
  if (formatCsv()) {
	 exit(1);
  }

#if 0
  // Parse the command line
  try { 
	 // the ' ' is the delimiter
	 TCLAP::CmdLine cmd(desc, ' ', "0.1");

	 TCLAP::ValueArg<std::string> vfArg("i", 
													"vector-file", 
													"The vectors file form Omnet", 
													true,
													"",
													"string");
	 cmd.add(vfArg);

	 TCLAP::ValueArg<std::string> vnfArg("n",
													 "vectors names file", 
													 "The object:vector pairs file", 
													 true, 
													 "",
													 "string");
	 cmd.add(vnfArg);

	 TCLAP::ValueArg<std::string> startArg("s", // short
														"start", // long 
														"start at the given time", // desc
														false, // required
														"0.0", // default 
														"double" // desc of type
														);
	 cmd.add(startArg);
	 
	 TCLAP::ValueArg<std::string> endArg("e", // short
													 "end", // long 
													 "end at the given time", // desc
													 false, // required
													 "-1.0", // default 
													 "double" // desc of type
													 );
	 cmd.add(endArg);

	 TCLAP::ValueArg<std::string> deltaArg("d", // short
														"delta-t", // long 
														"time between repoered", // desc
														false, // required
														"-1.0", // default 
														"double" // desc of type
														);
	 cmd.add(deltaArg);

	 TCLAP::SwitchArg statSw("q","statistics","only provide statistics", 
									 cmd, false);
	 
	 TCLAP::SwitchArg noSampSw("k",
										"no sample and hold",
										"output empty string instead of previous value",
										cmd, false);

	 TCLAP::SwitchArg verboseSw("v",
										 "verbose",
										 "Turn on verbose mode", 
										 cmd, false);

	 // Parse the argv array.
	 cmd.parse( argc, argv );
	 
	 // Get the value parsed by each arg. 
	 vecFileName = vfArg.getValue();
	 namesFileName = vnfArg.getValue();
	 start = atof(startArg.getValue().c_str());
	 end = atof(endArg.getValue().c_str());
	 delta = atof(deltaArg.getValue().c_str());
	 

	 noSample = noSampSw.getValue();
	 statMode = statSw.getValue();

	 verbose = verboseSw.getValue();

  } catch (TCLAP::ArgException &e) {
	 cerr << "error: " << e.error() << " for arg " << e.argId()
				  << endl; 
  }

#endif

  return(0);
}

