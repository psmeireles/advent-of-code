package org.example;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

class Input{
    public Input(ArrayList<Long> starts, ArrayList<Long> ends) {
        Starts = starts;
        Ends = ends;
    }

    ArrayList<Long> Starts = new ArrayList<>();
    ArrayList<Long> Ends = new ArrayList<>();
}

public class Main {
    static Input readFile(){
        ArrayList<Long> starts = new ArrayList<>();
        ArrayList<Long> ends = new ArrayList<>();
        var file = new File("Day02/input.txt");
        try (var scanner = new Scanner(file)){
            while(scanner.hasNextLine()){
                var line = scanner.nextLine();
                var intervals = line.split(",");
                for (var interval : intervals){
                    var split = interval.split("-");
                    starts.add(Long.parseLong(split[0]));
                    ends.add(Long.parseLong(split[1]));
                }
            }
        }
        catch (FileNotFoundException e){
            System.out.println("File not found");
            e.printStackTrace();
            return null;
        }

        return new Input(starts, ends);
    }
    static void main() {
        var input = readFile();
        var starts = input.Starts;
        var ends = input.Ends;

        // Part 1
        long result1 = 0;

        // For each range
        for (var i = 0; i < starts.size(); i++){
            var start = starts.get(i);
            var end = ends.get(i);

            // Go through each number, split in half and check if they're the same
            for (var j = start; j <= end; j++){
                var jString = String.valueOf(j);
                if (jString.length() % 2 == 1){
                    continue;
                }

                var firstHalf = jString.substring(jString.length()/2);
                var secondHalf = jString.substring(0, jString.length()/2);
                if (firstHalf.equals(secondHalf)){
                    result1 += j;
                    //System.out.println(j);
                }
            }
        }

        // Part 1: 30599400849
        System.out.println("Part 1: " + result1);

        // Part 2
        long result2 = 0;

        // For each range
        for (var i = 0; i < starts.size(); i++){
            var start = starts.get(i);
            var end = ends.get(i);

            // Go through each number
            for (var j = start; j <= end; j++){
                var jString = String.valueOf(j);

                // Try all sizes of repeating segments
                for (var k = 1; k <= jString.length()/2; k++){
                    // If size string isn't divisible by size, skip
                    if (jString.length() % k != 0){
                        continue;
                    }
                    var segment = jString.substring(0, k);
                    var isInvalid = true;
                    for (var l = 0; l < jString.length(); l++){
                        if (jString.charAt(l) != segment.charAt(l%k)){
                            isInvalid = false;
                            break;
                        }
                    }

                    if (isInvalid){
                        result2 += j;
                        //System.out.println(j);
                        break;
                    }
                }
            }
        }

        // Part 2: 46270373595
        System.out.println("Part 2: " + result2);
    }
}
