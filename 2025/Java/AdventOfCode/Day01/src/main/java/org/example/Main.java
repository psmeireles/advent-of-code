package org.example;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

class Input{
    public Input(ArrayList<String> directions, ArrayList<Integer> steps) {
        Directions = directions;
        Steps = steps;
    }

    ArrayList<String> Directions = new ArrayList<>();
    ArrayList<Integer> Steps = new ArrayList<>();
}

public class Main {
    static Input readFile(){
        ArrayList<String> directions = new ArrayList<>();
        ArrayList<Integer> steps = new ArrayList<>();
        var file = new File("Day01/input.txt");
        try (var scanner = new Scanner(file)){
            while(scanner.hasNextLine()){
                var line = scanner.nextLine();
                directions.add(line.substring(0, 1));
                steps.add(Integer.parseInt(line.substring(1)));
            }
        }
        catch (FileNotFoundException e){
            System.out.println("File not found");
            e.printStackTrace();
            return null;
        }

        return new Input(directions, steps);
    }
    static void main() {
        var input = readFile();
        var directions = input.Directions;
        var steps = input.Steps;

        var start = 50;
        var result1 = 0;

        // Part 1
        for (int i = 0; i < directions.size(); i++){
            start = switch (directions.get(i)) {
                case "R" -> (start + steps.get(i)) % 100;
                case "L" -> (((start - steps.get(i)) % 100) + 100) % 100;
                default -> start;
            };
            if (start == 0){
                result1++;
            }
        }

        // Part 1 result: 999
        System.out.println("Part 1: " + result1);

        // Part 2
        start = 50;
        var result2 = 0;
        for (int i = 0; i < directions.size(); i++){
            var step = steps.get(i);
            var remainder = step % 100;
            result2 += step / 100;
            switch (directions.get(i)) {
                case "R":
                    if (start != 0 && remainder >= 100 - start){
                        result2++;
                    }
                    start = (start + step) % 100;
                    break;
                case "L":
                    if (start != 0 && remainder >= start){
                        result2++;
                    }
                    start = (((start - step) % 100) + 100) % 100;
                    break;
            }
        }

        // Part 2 result: 6099
        System.out.println("Part 2: " + result2);
    }
}
