1.Complie pla2dot.cpp
%> g++ pla2dot.cpp -o robdd

2. Run ROBDD to convert the PLA file to ROBDD DOT file
%> ./robdd PLA_FILE DOT_FILE

3.visualize ROBDD DOT file
%> dot -T png DOT_FILE > PNG_NAME
