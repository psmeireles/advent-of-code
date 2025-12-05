package org.example;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Scanner;

class Input {
    public Input(ArrayList<ArrayList<Long>> freshRanges, ArrayList<Long> ingredients) {
        FreshRanges = freshRanges;
        Ingredients = ingredients;
    }

    ArrayList<ArrayList<Long>> FreshRanges;
    ArrayList<Long> Ingredients;
}

public class Main {
    static Input readFile() {
        ArrayList<ArrayList<Long>> freshRanges = new ArrayList<>();
        ArrayList<Long> ingredients = new ArrayList<>();
        var file = new File("Day05/input.txt");
        try (var scanner = new Scanner(file)) {
            var readingRanges = true;
            while (scanner.hasNextLine()) {
                var line = scanner.nextLine();
                if (line.isEmpty()){
                    readingRanges = false;
                    continue;
                }

                if (readingRanges) {
                    var split = line.split("-");
                    var numbers = new ArrayList<>(Arrays.asList(
                        Long.parseLong(split[0]),
                        Long.parseLong(split[1])
                    ));
                    freshRanges.add(numbers);
                }
                else {
                    ingredients.add(Long.parseLong(line));
                }
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
            e.printStackTrace();
            return null;
        }

        return new Input(freshRanges, ingredients);
    }

    static void main() {
        var input = readFile();
        if (input == null){
            return;
        }

        var ingredients = input.Ingredients;
        var ranges = input.FreshRanges;

        // Part 1
        var result1 = 0;
        for (var ingredient : ingredients) {
            for (var range : ranges) {
                if (range.getFirst() <= ingredient && range.getLast() >= ingredient){
                    result1++;
                    break;
                }
            }
        }

        // Output Part 1: 511
        System.out.println("Part 1: " + result1);

        // Part 2
        var result2 = 0L;

        // Sort ranges ascending by start, then end
        ranges.sort(Comparator.comparingLong((ArrayList<Long> a) -> a.getFirst()).thenComparingLong(ArrayList::getLast));

        // Go ascending in ranges, counting new items for each new range
        var lastEnd = -1L;
        for (var range : ranges) {
            var start = range.getFirst();
            var end = range.getLast();
            var newStart = Math.max(start, lastEnd + 1);
            var newEnd = Math.max(end, lastEnd);
            result2 += Math.max(0, newEnd - newStart + 1);
            lastEnd = newEnd;
        }

        // Output Part 2: 350939902751909
        System.out.println("Part 2: " + result2);
    }
}
