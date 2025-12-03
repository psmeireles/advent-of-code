package org.example;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class Main {
    static ArrayList<ArrayList<Integer>> readFile() {
        ArrayList<ArrayList<Integer>> banks = new ArrayList<>();
        var file = new File("Day03/input.txt");
        try (var scanner = new Scanner(file)) {
            while (scanner.hasNextLine()) {
                var line = scanner.nextLine();
                var bank = new ArrayList<Integer>();
                for (var letter : line.toCharArray()) {
                    bank.add(letter - '0');
                }
                banks.add(bank);
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
            e.printStackTrace();
            return null;
        }

        return banks;
    }

    static void main() {
        var banks = readFile();
        if (banks == null) {
            return;
        }

        // Part 1
        var result1 = 0;
        for (var bank : banks) {
            var max1 = Collections.max(bank.subList(0, bank.size() - 1));
            var max1Index = bank.indexOf(max1);
            var max2 = Collections.max(bank.subList(max1Index + 1, bank.size()));
            var bestPair = max1 * 10 + max2;
            System.out.println(bestPair);
            result1 += bestPair;
        }

        // Output Part 1: 17301
        System.out.println("Part 1: " + result1);


        // Part 2
        var result2 = 0L;
        for (var bank : banks) {
            var remaining = 12;
            var batteries = new ArrayList<>(bank);
            var result = new ArrayList<Integer>();
            while (remaining > 0){
                var leftEnd = batteries.size() - remaining + 1;
                var leftSide = batteries.subList(0, leftEnd);
                var max = Collections.max(leftSide);
                result.add(max);
                var maxIndex = batteries.indexOf(max);
                batteries = new ArrayList<Integer>(batteries.subList(maxIndex + 1, batteries.size()));
                remaining--;
            }

            var bestSelection = 0L;
            for (int i = 0; i < result.size(); i++){
                bestSelection += result.get(i) * Math.pow(10, result.size() - i - 1);
            }
            System.out.println(bestSelection);
            result2 += bestSelection;
        }

        // Output Part 2: 172162399742349
        System.out.println("Part 2: " + result2);
    }
}
