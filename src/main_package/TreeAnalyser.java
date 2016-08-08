package main_package;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class TreeAnalyser {

	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		
//		IntervalTreesList ITL = new IntervalTreesList();
		String joined = IntervalTreesList.joinTrees(
				"((node1[3-4])node0[5-7](node2[7-7](node3[11-11])))",
				"(node0[6-8](node2[8-10](node4[10-10])))");
		
		System.out.println(IntervalTreesList.isValidTree(
				"((node1[3-4])node0[5-7](node2[7-7](node3[11-11])))"));
		System.out.println(IntervalTreesList.isValidTree(
				"(node0[6-8](node2[8-10](node4[10-10])))"));
		System.out.println(IntervalTreesList.isValidTree(joined));
		System.out.println(joined);
		
//		//check if a file address was given
//		if(args.length < 1) {
//			System.out.println("File address wasn't specified.");
//			return;
//		}
//		
//		//get file address
//		String CodeFileAdd = args[0];
//		
//		try {
//			//open file and set readers
//			FileReader fr = new FileReader("C:\\Users\\עידן\\Documents\\test.txt");
//			BufferedReader br = new BufferedReader(fr);
//			
//			//create the abstract representation
//			IntervalTreesList ITL = new IntervalTreesList();
//		}
//		catch (IOException e) {
//			System.out.println("Invalid File.");
//			return;
//		}

	}

}
