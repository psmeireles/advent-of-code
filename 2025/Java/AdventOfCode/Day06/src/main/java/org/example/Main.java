package org.example;


import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

class Input {
    public Input(ArrayList<ArrayList<Long>> numbers, ArrayList<String> operations) {
        Numbers = numbers;
        Operations = operations;
    }

    ArrayList<ArrayList<Long>> Numbers;
    ArrayList<String> Operations;
}


public class Main {
    static ArrayList<String> normalizeSpacesAndSplit(String s) {
        return new ArrayList<>(Arrays.asList(s.trim().replaceAll(" +", " ").split(" ")));
    }
    static Input readFile1() {
        ArrayList<ArrayList<Long>> numbers = new ArrayList<>();
        ArrayList<String> operations = new ArrayList<>();
        var file = new File("Day06/input.txt");
        try (var scanner = new Scanner(file)) {
            while (scanner.hasNextLine()) {
                var line = scanner.nextLine();

                var splitNumbers = normalizeSpacesAndSplit(line);

                // Handle operations line
                if (line.contains("+") || line.contains("*")){
                    operations = splitNumbers;
                    continue;
                }

                // Handle numbers lines
                var rowNumbers = new ArrayList<Long>();
                for (var numStr : splitNumbers){
                    rowNumbers.add(Long.parseLong(numStr));
                }
                numbers.add(rowNumbers);
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
            e.printStackTrace();
            return null;
        }

        return new Input(numbers, operations);
    }

    static Input readFile2() {
        ArrayList<String> lines = new ArrayList<>();
        var file = new File("Day06/input.txt");
        try (var scanner = new Scanner(file)) {
            while (scanner.hasNextLine()) {
                lines.add(scanner.nextLine());
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
            e.printStackTrace();
            return null;
        }

        ArrayList<String> operations = normalizeSpacesAndSplit(new StringBuilder(lines.getLast()).reverse().toString());

        var numRows = lines.size() - 1;
        var numCols = lines.getFirst().length();
        ArrayList<ArrayList<Long>> numbers = new ArrayList<>();
        ArrayList<Long> operationNumbers = new ArrayList<>();
        for (int j = numCols - 1; j >= 0; j--) {
            var numStr = new StringBuilder();
            for (int i = 0; i < numRows; i++){
                var charAt = lines.get(i).charAt(j);
                if (charAt == ' '){
                    continue;
                }
                numStr.append(charAt);
            }

            // Found empty column. Store current numbers as a group and continue
            if (numStr.isEmpty()) {
                numbers.add(new ArrayList<>(operationNumbers));
                operationNumbers.clear();
                continue;
            }

            operationNumbers.add(Long.parseLong(numStr.toString()));

            // If first column, append the last group
            if (j == 0) {
                numbers.add(new ArrayList<>(operationNumbers));
            }
        }


        return new Input(numbers, operations);
    }
    
    static void main() {
        // Part 1
        {
            var input = readFile1();
            if (input == null){
                return;
            }
            var operations = input.Operations;
            var numbers = input.Numbers;

            for (var n : numbers){
                System.out.println(n);
            }
            System.out.println(operations);

            var result1 = 0L;
            for (int i = 0; i < operations.size(); i++){
                var op = operations.get(i);
                var result = numbers.getFirst().get(i);
                for (int j = 1; j < numbers.size(); j++){
                    if (op.equals("+")) {
                        result += numbers.get(j).get(i);
                    } else if (op.equals("*")) {
                        result *= numbers.get(j).get(i);
                    }
                }
                result1 += result;
            }

            // Output Part 1: 4878670269096
            System.out.println("Part 1: " + result1);
        }

        // Part 2
        {
            var input = readFile2();
            if (input == null){
                return;
            }
            var operations = input.Operations;
            var numbers = input.Numbers;

            for (var n : numbers){
                System.out.println(n);
            }
            System.out.println(operations);

            var result2 = 0L;
            for (int i = 0; i < operations.size(); i++){
                var op = operations.get(i);
                var result = numbers.get(i).getFirst();
                for (int j = 1; j < numbers.get(i).size(); j++){
                    if (op.equals("+")) {
                        result += numbers.get(i).get(j);
                    } else if (op.equals("*")) {
                        result *= numbers.get(i).get(j);
                    }
                }
                result2 += result;
            }

            // Output Part 2: 8674740488592
            System.out.println("Part 2: " + result2);
        }
    }
}
