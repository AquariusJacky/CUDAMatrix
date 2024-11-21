#include "MatrixTester.h"

MatrixTester::MatrixTester() {}
MatrixTester::~MatrixTester() {
  for (int i = 0; i < testerVec.size(); i++) {
    delete testerVec[i];
  }
}

void MatrixTester::createTest(const std::string& test_name,
                              const std::string& operation_name,
                              const Matrix& mat, const bool& isCUDA) {
  if (getOperationType().count(operation_name) == 0) {
    std::cout << "In " << test_name << ", " << operation_name
              << " is not a valid operation." << std::endl;
    return;
  }
  std::unordered_map<std::string, int> opertaionType = getOperationType();
  int operation_id = opertaionType[operation_name];

  RunningUnit* newTester = new RunningUnit;
  if (newTester->init(test_name, operation_id, mat, isCUDA) == 1) {
    delete newTester;
  }

  std::cout << "Created test \"" << test_name
            << "\" with operation: " << operation_name << "." << std::endl;
  testerVec.push_back(newTester);
}

void MatrixTester::createTest(const std::string& test_name,
                              const std::string& operation_name,
                              const Matrix& mat, const float& val,
                              const bool& isCUDA) {
  if (getOperationType().count(operation_name) == 0) {
    std::cout << "In " << test_name << ", " << operation_name
              << " is not a valid operation." << std::endl;
    return;
  }
  std::unordered_map<std::string, int> opertaionType = getOperationType();
  int operation_id = opertaionType[operation_name];

  RunningUnit* newTester = new RunningUnit;
  if (newTester->init(test_name, operation_id, mat, val, isCUDA) == 1) {
    delete newTester;
  }

  std::cout << "Created test \"" << test_name
            << "\" with operation: " << operation_name << "." << std::endl;
  testerVec.push_back(newTester);
}

void MatrixTester::createTest(const std::string& test_name,
                              const std::string& operation_name,
                              const Matrix& matA, const Matrix& matB,
                              const bool& isCUDA) {
  if (getOperationType().count(operation_name) == 0) {
    std::cout << "In " << test_name << ", " << operation_name
              << " is not a valid operation." << std::endl;
    return;
  }
  std::unordered_map<std::string, int> opertaionType = getOperationType();
  int operation_id = opertaionType[operation_name];

  RunningUnit* newTester = new RunningUnit;
  if (newTester->init(test_name, operation_id, matA, matB, isCUDA) == 1) {
    delete newTester;
  }

  std::cout << "Created test \"" << test_name
            << "\" with operation: " << operation_name << "." << std::endl;
  testerVec.push_back(newTester);
}

void MatrixTester::runTest(const std::string& test_name) {
  int testerID = findTester(test_name);
  if (testerID == -1) return;
  runTest(testerID);
}

void MatrixTester::printResult(const std::string& name) {
  int testerID = findTester(name);
  if (testerID == -1) return;
  printResult(testerID);
}

void MatrixTester::printError(const std::string& nameA,
                              const std::string& nameB) {
  int testerAID = findTester(nameA);
  int testerBID = findTester(nameB);
  if (testerAID == -1 || testerBID == -1) return;
  printError(testerAID, testerBID);
}

void MatrixTester::printTime(const std::string& name) {
  int testerID = findTester(name);
  if (testerID == -1) return;
  printTime(testerID);
}

void MatrixTester::runTest(const int& testerID) {
  if (isValidTesterID(testerID) == 0) return;
  testerVec[testerID]->run();
}

void MatrixTester::printResult(const int& testerID) {
  if (isValidTesterID(testerID) == 0) return;
  std::cout << "\"" << testerVec[testerID]->name() << "\" result:\n"
            << testerVec[testerID]->result();
}

void MatrixTester::printError(const int& testerAID, const int& testerBID) {
  if (isValidTesterID(testerAID) == 0 || isValidTesterID(testerBID) == 0)
    return;

  Matrix matA = testerVec[testerAID]->result();
  Matrix matB = testerVec[testerBID]->result();
  if (matA.m() != matB.m() || matA.n() != matB.n()) {
    std::cout << "Matrix dimensions do not match" << std::endl;
  }
  Matrix diff = matA - matB;
  float error = diff.sum();

  std::cout << "Error between " << testerVec[testerAID]->name() << " and "
            << testerVec[testerBID]->name() << ": " << error << std::endl;
}

void MatrixTester::printTime(const int& testerID) {
  if (isValidTesterID(testerID) == 0) return;
  std::cout << "\"" << testerVec[testerID]->name()
            << "\" runtime: " << testerVec[testerID]->time() << " ms"
            << std::endl;
}

int MatrixTester::isValidTesterID(const int& testerID) {
  if (testerID < 0 || testerID >= testerVec.size()) {
    std::cout << "Invalid tester ID: " << testerID << std::endl;
    return 0;
  }
  return 1;
}

int MatrixTester::findTester(const std::string& name) {
  int testerID = -1;
  for (int i = 0; i < testerVec.size(); i++) {
    if (testerVec[i]->name() == name) testerID = i;
  }
  if (testerID == -1)
    std::cout << "Can't find tester with name \"" << name << "\"" << std::endl;

  return testerID;
}