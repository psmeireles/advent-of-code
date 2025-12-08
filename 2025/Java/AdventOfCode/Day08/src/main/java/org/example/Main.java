package org.example;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

class Point {
    long X;
    long Y;
    long Z;

    public Point(long x, long y, long z) {
        X = x;
        Y = y;
        Z = z;
    }

    @Override
    public String toString() {
        return "(" + X +
                ", " + Y +
                ", " + Z +
                ')';
    }

    public long DistanceSquared(Point p) {
        var dx = X - p.X;
        var dy = Y - p.Y;
        var dz = Z - p.Z;

       return dx*dx + dy*dy + dz*dz;
    }
}

class UnionFind {
    HashMap<Point, Point> parent = new HashMap<>();
    HashMap<Point, Integer> rank = new HashMap<>();

    public void Union(Point a, Point b) {
        var root1 = Find(a);
        var root2 = Find(b);
        if (root1 != root2){
            var rank1 = rank.get(root1);
            var rank2 = rank.get(root2);
            if (rank1 < rank2) {
                parent.put(root1, root2);
            }
            else if (rank1 > rank2) {
                parent.put(root2, root1);
            }
            else {
                parent.put(root1, root2);
                rank.put(root1, rank1 + 1);
            }
        }
    }

    public Point Find(Point a) {
        if (!parent.containsKey(a)){
            parent.put(a, a);
            rank.put(a, 0);
            return a;
        }

        // Compact
        var myParent = parent.get(a);
        if (myParent != a) {
            parent.put(a, Find(myParent));
        }

        return parent.get(a);
    }
}

public class Main {
    static ArrayList<Point> readFile() {
        ArrayList<Point> points = new ArrayList<>();
        var file = new File("Day08/input.txt");
        try (var scanner = new Scanner(file)) {
            while (scanner.hasNextLine()) {
                var line = scanner.nextLine();
                var coords = line.split(",");
                points.add(new Point(Long.parseLong(coords[0]), Long.parseLong(coords[1]), Long.parseLong(coords[2])));
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
            e.printStackTrace();
            return null;
        }

        return points;
    }

    static void main() {
        var points = readFile();
        if (points == null) {
            return;
        }

        // Generate all pairs and sort
        var sortedPairs = new ArrayList<ArrayList<Point>>();
        for (int i = 0; i < points.size(); i++){
            for (int j = i + 1; j < points.size(); j++){
                var pair = new ArrayList<Point>();
                pair.add(points.get(i));
                pair.add(points.get(j));
                sortedPairs.add(pair);
            }
        }

        sortedPairs.sort(Comparator.comparingLong(a -> a.getFirst().DistanceSquared(a.getLast())));

        var lastUnionResult = 0L;
        var uf = new UnionFind();
        for (int i = 0; i < sortedPairs.size(); i++){
            if (i == 1000) {
                // Output Part 1: 97384
                printPart1Output(points, uf);
            }

            var pair = sortedPairs.get(i);
            var point1 = pair.getFirst();
            var point2 = pair.getLast();

            if (uf.Find(point1) != uf.Find(point2)){
                uf.Union(point1, point2);
                lastUnionResult = point1.X * point2.X;
            }
        }

        // Part 2
        System.out.println("Part 2: " + lastUnionResult);
    }

    private static void printPart1Output(ArrayList<Point> points, UnionFind uf) {
        // Find circuit sizes
        var circuitSizes = new HashMap<Point, Integer>();
        for (var point : points) {
            var root = uf.Find(point);
            if (!circuitSizes.containsKey(root)) {
                circuitSizes.put(root, 0);
            }
            circuitSizes.put(root, circuitSizes.get(root) + 1);
        }

        // Find 3 largest circuits and multiply sizes
        var list = new ArrayList<>(circuitSizes.entrySet());
        list.sort(Map.Entry.comparingByValue());
        list = new ArrayList<>(list.reversed());

        var result1 = list.get(0).getValue() * list.get(1).getValue() * list.get(2).getValue();

        // Output Part 1: 97384
        System.out.println("Part 1: " + result1);
    }
}
