package org.example;

import java.awt.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class Main {
    static ArrayList<String> readFile() {
        ArrayList<String> lines = new ArrayList<>();
        var file = new File("Day07/input.txt");
        try (var scanner = new Scanner(file)) {
            while (scanner.hasNextLine()) {
                var line = scanner.nextLine();
                lines.add(line);
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
            e.printStackTrace();
            return null;
        }

        return lines;
    }

    static void main() {
        var grid = readFile();
        if (grid == null) {
            return;
        }

        var start = getStartPosition(grid);

        // Part 1
        var result1 = getSplits(grid, start[0], start[1]);

        // Output Part 1: 1656
        System.out.println("Part 1: " + result1);

        // Part 2
        var result2 = getTimelines(grid, start[0], start[1]);

        // Output Part 2: 76624086587804
        System.out.println("Part 2: " + result2);
    }

    private static int[] getStartPosition(ArrayList<String> grid) {
        for (int i = 0; i < grid.size(); i++){
            for (int j = 0; j < grid.get(i).length(); j++){
                if (grid.get(i).charAt(j) == 'S') {
                    return new int[]{i, j};
                }
            }
        }
        return new int[]{-1, -1};
    }

    static HashSet<String> splitMemo = new HashSet<>();
    private static int getSplits(ArrayList<String> grid, int i, int j) {
        if (i < 0 || j < 0 || i >= grid.size() || j >= grid.get(i).length()) {
            return 0;
        }

        for (int x = i; x < grid.size(); x++) {
            if (grid.get(x).charAt(j) == '^'){
                var pointString = x + "-" + j;
                if (!splitMemo.contains(pointString)) {
                    splitMemo.add(pointString);
                    return 1 + getSplits(grid, x, j-1) + getSplits(grid, x, j + 1);
                }

                // Already counted this split. Ignore.
                return 0;
            }
        }

        return 0;
    }

    static HashMap<String, Long> timelinesMemo = new HashMap<>();
    private static Long getTimelines(ArrayList<String> grid, int i, int j) {
        if (i < 0 || j < 0 || i >= grid.size() || j >= grid.get(i).length()) {
            return 0L;
        }

        var pointString = i + "-" + j;
        if (timelinesMemo.containsKey(pointString)) {
            return timelinesMemo.get(pointString);
        }

        timelinesMemo.put(pointString, 1L);
        for (int x = i; x < grid.size(); x++) {
            if (grid.get(x).charAt(j) == '^'){
                var result = getTimelines(grid, x, j-1) + getTimelines(grid, x, j + 1);
                timelinesMemo.put(pointString, result);
                break;
            }
        }

        return timelinesMemo.get(pointString);
    }
}
