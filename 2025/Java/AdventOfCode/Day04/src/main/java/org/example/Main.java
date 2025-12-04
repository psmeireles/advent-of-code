package org.example;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class Main {

    static ArrayList<String> readFile() {
        ArrayList<String> lines = new ArrayList<>();
        var file = new File("Day04/input.txt");
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

    static int numNeighbors(ArrayList<String> grid, int x, int y) {
        int[][] directions = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1},};

        var numNeighbours = 0;
        for (var dir : directions) {
            var newX = x + dir[0];
            var newY = y + dir[1];

            if (newX >= 0 && newY >= 0 && newX < grid.size() && newY < grid.get(newX).length() && grid.get(newX).charAt(newY) == '@') {
                numNeighbours++;
            }
        }
        return numNeighbours;
    }

    static void main() {
        var grid = readFile();
        if (grid == null) {
            return;
        }

        // Part 1
        var result1 = 0;
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid.get(i).length(); j++) {
                if (grid.get(i).charAt(j) == '@' && numNeighbors(grid, i, j) < 4) {
                    result1++;
                }
            }
        }

        // Output Part 1: 1553
        System.out.println("Part 1: " + result1);

        // Part 2
        var result2 = 0;
        boolean removedThisIteration;
        do {
            removedThisIteration = false;
            for (int i = 0; i < grid.size(); i++) {
                for (int j = 0; j < grid.get(i).length(); j++) {
                    if (grid.get(i).charAt(j) == '@' && numNeighbors(grid, i, j) < 4) {
                        // Remove toilet paper from that position
                        grid.set(i, grid.get(i).substring(0, j) + '.' + grid.get(i).substring(j + 1));
                        removedThisIteration = true;
                        result2++;
                    }
                }
            }
        }
        while (removedThisIteration);

        // Output Part 2: 8442
        System.out.println("Part 2: " + result2);
    }
}
