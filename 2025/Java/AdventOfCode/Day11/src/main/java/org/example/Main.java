package org.example;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Scanner;

public class Main {
    static HashMap<String, List<String>> readFile() {
        var graph = new HashMap<String, List<String>>();
        var file = new File("Day11/input.txt");
        try (var scanner = new Scanner(file)) {
            while (scanner.hasNextLine()) {
                var line = scanner.nextLine();
                var items = line.split(" ");
                var key = items[0].substring(0, 3);
                for (int i = 1; i < items.length; i++) {
                    if (!graph.containsKey(key)) {
                        graph.put(key, new ArrayList<>());
                    }
                    graph.get(key).add(items[i]);
                }
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
            e.printStackTrace();
            return null;
        }

        return graph;
    }

    static void main() {
        var graph = readFile();
        if (graph == null) {
            return;
        }

        System.out.println(graph);

        // Part 1: 607
        var result1 = getNumPathsBetweenPoints(graph, "you", "out", new HashMap<>());
        System.out.println("Part 1: " + result1);

        // Part 2: 506264456238938
        var svr_fft = getNumPathsBetweenPoints(graph, "svr", "fft", new HashMap<>());
        var svr_dac = getNumPathsBetweenPoints(graph, "svr", "dac", new HashMap<>());
        var dac_fft = getNumPathsBetweenPoints(graph, "dac", "fft", new HashMap<>());
        var fft_dac = getNumPathsBetweenPoints(graph, "fft", "dac", new HashMap<>());
        var fft_out = getNumPathsBetweenPoints(graph, "fft", "out", new HashMap<>());
        var dac_out = getNumPathsBetweenPoints(graph, "dac", "out", new HashMap<>());

        var result2 = svr_fft*fft_dac*dac_out + svr_dac*dac_fft*fft_out;
        System.out.println("Part 2: " + result2);
    }

    static long getNumPathsBetweenPoints(HashMap<String, List<String>> graph, String start, String end, HashMap<String, Long> memo){
        if (start.equals(end)) {
            return 1;
        }

        if (memo.containsKey(start)){
            return memo.get(start);
        }

        if (!graph.containsKey(start)){
            return 0;
        }

        var numPaths = 0L;
        for (var neighbor : graph.get(start)){
            numPaths += getNumPathsBetweenPoints(graph, neighbor, end, memo);
        }
        memo.put(start, numPaths);
        return numPaths;
    }
}
