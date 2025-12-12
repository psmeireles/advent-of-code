package org.example;

import java.awt.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

class Input {
    ArrayList<Shape> Shapes;
    ArrayList<Region> Regions;

    @Override
    public String toString() {
        return "Input{" +
                "Shapes=" + Shapes +
                ", Regions=" + Regions +
                '}';
    }

    public Input(ArrayList<Shape> shapes, ArrayList<Region> regions) {
        Shapes = shapes;
        Regions = regions;
    }
}

class Shape {
    int Area;

    @Override
    public String toString() {
        return "Shape{" +
                "Area=" + Area +
                '}';
    }

    public Shape(int area) {
        Area = area;
    }
}

class Region {
    int Width;
    int Height;
    int[] Quantities;

    @Override
    public String toString() {
        return "Region{" +
                "Width=" + Width +
                ", Height=" + Height +
                ", Quantities=" + Arrays.toString(Quantities) +
                '}';
    }

    public Region(int width, int height, int[] quantities) {
        Width = width;
        Height = height;
        Quantities = quantities;
    }

}

public class Main {
    static Input readFile() {
        var shapes = new ArrayList<Shape>();
        var regions = new ArrayList<Region>();
        var file = new File("Day12/input.txt");
        try (var scanner = new Scanner(file)) {
            while (scanner.hasNextLine()) {
                var line = scanner.nextLine();
                if (line.endsWith(":")) {
                    // Start of Shape block
                    var blockArea = 0;
                    while (scanner.hasNextLine()) {
                        var nextLine = scanner.nextLine();
                        if (nextLine.isEmpty()) {
                            // Reached end of block
                            shapes.add(new Shape(blockArea));
                            break;
                        }
                        for (int i = 0; i < nextLine.length(); i++){
                            if (nextLine.charAt(i) == '#') {
                                blockArea++;
                            }
                        }
                    }
                }
                else {
                    // Regions
                    var splitByColon = line.split(":");
                    var dimensions = splitByColon[0].split("x");
                    var width = Integer.parseInt(dimensions[0]);
                    var height = Integer.parseInt(dimensions[1]);
                    var quantities = Arrays.stream(splitByColon[1].substring(1).split(" ")).mapToInt(Integer::parseInt).toArray();
                    regions.add(new Region(width, height, quantities));
                }
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
            e.printStackTrace();
            return null;
        }

        return new Input(shapes, regions);
    }

    static void main() {
        var input = readFile();
        if (input == null) {
            return;
        }

        var regions = input.Regions;
        var shapes = input.Shapes;

        var result = 0;
        // Luckily for this input we can just check if the needed area fits in the region
        for (var r : regions){
            var neededArea = 0;
            for (int i = 0; i < r.Quantities.length; i++) {
                neededArea += shapes.get(i).Area * r.Quantities[i];
            }

            if (neededArea <= r.Height * r.Width){
                result++;
            }
        }

        // Output: 505
        System.out.println("Result: " + result);
    }
}
