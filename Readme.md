# Iteration 1 SPA


## 1. Running AutoTester on macOS with CLion

1. Open CLion and navigate to the `Code_A0265565U` folder.
2. Use CLion to load the `CMakeLists.txt` file.
3. Locate the AutoTester configuration on the top right hand corner.
4. Edit the profile and set the working directory to `Tests_A0265565U`
5. Set argument in the configuration to the below
6. The output file will be located in the `Tests_A0265565U` folder. To view the output, use any compatible XML viewer or open it in a web browser.

```
iteration1_source.txt iteration1_queries.txt iteration1_out.xml
```
5. Next, click on the 'Run' for configuration AutoTester to start it
6. Alternatively you cabn also run
```
AutoTester ../Tests_A0265565U/iteration1_source.txt ../Tests_A0265565U/iteration1_queries.txt ../Tests_A0265565U/iteration1_out.xml
```

## 2. Running Unit Tests with Catch2

1. In CLion, navigate to the `unit_test` folder within your project.
2. Load the `CMakeLists.txt` file in the `src/unit_testing` folder.
3. In the same configuration profile on the top right hand side you should see run unit_testing
4. Clock on `run` to start testing all unit tests 
