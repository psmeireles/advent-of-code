package org.example;

import java.awt.*;
import java.awt.geom.Area;
import java.awt.geom.Path2D;
import java.awt.geom.Rectangle2D;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;
import java.util.List;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class Main {
    static List<Point> readFile() {
        var points = new ArrayList<Point>();
        var file = new File("Day09/input.txt");
        try (var scanner = new Scanner(file)) {
            while (scanner.hasNextLine()) {
                var line = scanner.nextLine();
                var coords = line.split(",");
                points.add(new Point(Integer.parseInt(coords[0]), Integer.parseInt(coords[1])));
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
            e.printStackTrace();
            return null;
        }

        return points;
    }

    static long Area(Rectangle r){
        return (long) r.height * (long) r.width;
    }

    static void main() {
        var redPoints = readFile();
        if (redPoints == null) {
            return;
        }

        // Part 1
        var sortedRectangles = new ArrayList<Rectangle>();
        for (int i = 0; i < redPoints.size(); i++){
            var pointI = redPoints.get(i);
            for (int j = i+1; j < redPoints.size(); j++){
                var pointJ = redPoints.get(j);
                var width = Math.abs(pointJ.x - pointI.x) + 1;
                var height = Math.abs(pointJ.y - pointI.y) + 1;
                sortedRectangles.add(new Rectangle(pointI.x, pointI.y, width, height));
            }
        }

        sortedRectangles.sort(Comparator.comparingLong(r -> -Area(r)));

        // Output Part 1: 4761736832
        var result1 = Area(sortedRectangles.getFirst());
        System.out.println("Part 1: " + result1);

        // Part 2
        var result2 = 0;

        // Output Part 2:
        System.out.println("Part 2: " + result2);
    }
}
