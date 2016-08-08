package main_package;

import java.util.LinkedList;
import java.util.List;

public class IntervalTreesList {

	private static int newNodeCounter = 0;
	
	//a state is represented by a list of strings, each one represents a tree
	public List<String> list;
	
	public IntervalTreesList() {
		this.list = new LinkedList<String>();
	}
	
	/*
	 * Get the tree with the specified root from the list.
	 * Return null if such tree wasn't found
	 */
	public String treeOf(String root) {
		for(String t : list) {
			if(root(t).equals(root)) return t;
		}
		return null;
	}
	
	public IntervalTreesList joinLists(IntervalTreesList otherITL) {
		IntervalTreesList joined = new IntervalTreesList();
		String otherTree = null;
		
		for(String t : list) {
			otherTree = otherITL.treeOf(root(t));
			joined.list.add(joinTrees(t, otherTree));
		}
		
		for(String t : otherITL.list) {
			if(this.treeOf(root(t)) == null)
				joined.list.add(t);
		}
		
		return joined;
	}

	 // Given a tree and a location of a node name, return the node name.
	public static String nodeName(String treeRep, int nameStart) {
		for(int i=nameStart; i<treeRep.length(); i++) {
			if(treeRep.charAt(i) == '[')
				return treeRep.substring(nameStart, i);
		}
		return null;
	}
	
	public static String joinTrees(String rep1, String rep2) {
		if(rep1 == null || rep1.equals("")) return rep2;
		if(rep2 == null || rep2.equals("")) return rep1;
		
		String left1, left2, right1, right2, root1, root2;
		int rootLB1, rootLB2, rootUB1, rootUB2, rootLoc1, rootLoc2;
		left1 = leftSubTree(rep1);
		left2 = leftSubTree(rep2);
		right1 = rightSubTree(rep1);
		right2 = rightSubTree(rep2);
		root1 = root(rep1);
		root2 = root(rep2);
		rootLoc1 = rootStart(rep1);
		rootLoc2 = rootStart(rep2);
		rootLB1 = lowerBound(rep1, rootLoc1);
		rootLB2 = lowerBound(rep2, rootLoc2);
		rootUB1 = upperBound(rep1, rootLoc1);
		rootUB2 = upperBound(rep2, rootLoc2);
		
		String left = joinTrees(left1, left2);
		String right = joinTrees(right1, right2);
		String root;
		int rootLB = Math.min(rootLB1, rootLB2);
		int rootUB = Math.max(rootUB1, rootUB2);
		if(root1.equals(root2)) root = root1;
		else {
			root = "n" + newNodeCounter;
			newNodeCounter++;
		}
		
		return "(" + left + root + "[" + rootLB + "-" + rootUB + "]" + right + ")";
	}
	
	 // Find the root of the tree, and return the location of its name.
	public static int rootStart(String rep) {
		int count = 0;
		
		for(int i=0; i<rep.length(); i++) {
			if(rep.charAt(i) == '(') { count++; continue; }
			if(rep.charAt(i) == ')') { count--; continue; }
			if(count == 1)  return i;
		}
		return -1;
	}
	
	//Get the full name of the root node of the tree
	public static String root(String rep) {
		int rs = rootStart(rep);
		
		if(rs < 0) return "";
		else return nodeName(rep, rs);
	}
	
	//Get the representation of the left subtree
	public static String leftSubTree(String rep) {
		int rs = rootStart(rep);
		
		if(rs < 0) return "";
		else return rep.substring(1, rs);
	}
	
	//Get the representation of the right subtree
	public static String rightSubTree(String rep) {
		int rs = rootStart(rep);
		
		if(rs < 0) return "";
		for(int i=rs; i<rep.length(); i++) {
			if(rep.charAt(i) == '(') return rep.substring(i, rep.length()-1);
		}
		return "";
	}
	
	public static int lowerBound(String rep, int nodeLocation) {
		for(int i=nodeLocation; i<rep.length(); i++) {
			if(rep.charAt(i) == '[') {
				for(int j=i; j<rep.length(); j++) {
					if(rep.charAt(j) == '-')
						return Integer.parseInt(rep.substring(i+1, j));
				}
				return -1;
			}
		}
		return -1;
	}
	
	public static int upperBound(String rep, int nodeLocation) {
		for(int i=nodeLocation; i<rep.length(); i++) {
			if(rep.charAt(i) == '-') {
				for(int j=i; j<rep.length(); j++) {
					if(rep.charAt(j) == ']')
						return Integer.parseInt(rep.substring(i+1, j));
				}
				return -1;
			}
		}
		return -1;
	}
	
	public static boolean isValidTree(String rep) {
		int[] numArray = new int[rep.length()];
		int index = 0;
		
		for(int i=0; i<rep.length(); i++) {
			if(rep.charAt(i) == '[') {
				numArray[index] = lowerBound(rep, i);
				numArray[index+1] = upperBound(rep, i);
				index += 2;
			}
		}
		
		for(int j=0; j<index-1; j++) {
			if(numArray[j] > numArray[j+1]) return false;
		}
		return true;
	}
}
