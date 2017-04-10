package problems.qbfac.solvers;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import metaheuristics.grasp.AbstractGRASP;
import problems.qbf.solvers.GRASP_QBF;
import problems.qbfac.QBFAC_Inverse;
import solutions.Solution;



/**
 * Metaheuristic GRASP (Greedy Randomized Adaptive Search Procedure) for
 * obtaining an optimal solution to a QBF (Quadractive Binary Function --
 * {@link #QuadracticBinaryFunction}). Since by default this GRASP considers
 * minimization problems, an inverse QBF function is adopted.
 * 
 * @author ccavellucci, fusberti
 */
public class GRASP_QBFAC extends GRASP_QBF {

	/**
	 * Constructor for the GRASP_QBF class. An inverse QBF objective function is
	 * passed as argument for the superclass constructor.
	 * 
	 * @param alpha
	 *            The GRASP greediness-randomness parameter (within the range
	 *            [0,1])
	 * @param iterations
	 *            The number of iterations which the GRASP will be executed.
	 * @param filename
	 *            Name of the file for which the objective function parameters
	 *            should be read.
	 * @throws IOException
	 *             necessary for I/O operations.
	 */
	public GRASP_QBFAC(Double alpha, Integer iterations, String filename, Boolean bestImproving) throws IOException {
		super(alpha, iterations, filename);
                this.bestImproving = bestImproving;
	}
        
        public GRASP_QBFAC(Double alpha, Long timeLimit, String filename, Boolean bestImproving) throws IOException {
		super(alpha, timeLimit, filename);
                this.bestImproving = bestImproving;
	}
        
        protected Boolean bestImproving;

	/*
	 * (non-Javadoc)
	 * 
	 * @see grasp.abstracts.AbstractGRASP#updateCL()
	 */
	@Override
	public void updateCL() {

		this.CL = buildCLBasedOnSolution(incumbentSol);

	}

        @Override
        protected Double chooseAlpha() {
            return alpha;
        }
        
        private ArrayList<Integer> buildCLBasedOnSolution(Solution<Integer> solution) {
                ArrayList<Integer> _cl = new ArrayList<Integer>();
                Collections.sort(solution);
                int j = 0;
                for(Integer i : solution) {
                        while(i > j + 1) {
                                _cl.add(j);
                                j++;
                        }
                        j = i + 2;
                }
                while(j < ObjFunction.getDomainSize()) {
                        _cl.add(j);
                        j++;
                }
                if(!bestImproving) {
                        Collections.shuffle(_cl, rng);
                }
                return _cl;
        }
        
	/**
	 * {@inheritDoc}
	 * 
	 * The local search operator developed for the QBF objective function is
	 * composed by the neighborhood moves Insertion, Removal and 2-Exchange.
	 */
	@Override
	public Solution<Integer> localSearch() {

		Double minDeltaCost;
		Integer bestCandIn = null, bestCandOut = null;

		do {
			minDeltaCost = Double.POSITIVE_INFINITY;
			updateCL();
                        List<Integer> operationOrder = Arrays.asList(0, 1, 2);
                        Collections.shuffle(operationOrder, rng);
			
                        for(int i = 0; i < operationOrder.size(); i++) {
                                if(operationOrder.get(i) == 0) {
                                        // Evaluate insertions
                                        for (Integer candIn : CL) {
                                                double deltaCost = ObjFunction.evaluateInsertionCost(candIn, incumbentSol);
                                                if (deltaCost < minDeltaCost) {
                                                        minDeltaCost = deltaCost;
                                                        bestCandIn = candIn;
                                                        bestCandOut = null;
                                                        if(!bestImproving) {
                                                                incumbentSol.add(bestCandIn);
                                                                CL.remove(bestCandIn);
                                                                ObjFunction.evaluate(incumbentSol);
                                                                return null;
                                                        }
                                                }
                                        }
                                }
                                else if(operationOrder.get(i) == 1) {
                                        // Evaluate removals
                                        for (Integer candOut : incumbentSol) {
                                                double deltaCost = ObjFunction.evaluateRemovalCost(candOut, incumbentSol);
                                                if (deltaCost < minDeltaCost) {
                                                        minDeltaCost = deltaCost;
                                                        bestCandIn = null;
                                                        bestCandOut = candOut;
                                                        if(!bestImproving) {
                                                                incumbentSol.remove(bestCandOut);
                                                                CL.add(bestCandOut);
                                                                ObjFunction.evaluate(incumbentSol);
                                                                return null;
                                                        }
                                                }
                                        }
                                }
                                else if(operationOrder.get(i) == 2) {
                                        // Evaluate exchanges
                                        for (Integer candOut : incumbentSol) {
                                                Solution<Integer> nextSol = new Solution();
                                                for(Integer cand : incumbentSol)
                                                {
                                                    if(cand != candOut)
                                                        nextSol.add(cand);
                                                }
                                                ArrayList<Integer> newCL = buildCLBasedOnSolution(nextSol);
                                                for (Integer candIn : newCL) {
                                                        double deltaCost = ObjFunction.evaluateExchangeCost(candIn, candOut, incumbentSol);
                                                        if (deltaCost < minDeltaCost) {
                                                                minDeltaCost = deltaCost;
                                                                bestCandIn = candIn;
                                                                bestCandOut = candOut;
                                                                if(!bestImproving) {
                                                                        incumbentSol.add(bestCandIn);
                                                                        CL.remove(bestCandIn);
                                                                        incumbentSol.remove(bestCandOut);
                                                                        CL.add(bestCandOut);
                                                                        ObjFunction.evaluate(incumbentSol);
                                                                        return null;
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }
			// Implement the best move, if it reduces the solution cost.
			if (minDeltaCost < -Double.MIN_VALUE) {
				if (bestCandOut != null) {
					incumbentSol.remove(bestCandOut);
					CL.add(bestCandOut);
				}
				if (bestCandIn != null) {
					incumbentSol.add(bestCandIn);
					CL.remove(bestCandIn);
				}
				ObjFunction.evaluate(incumbentSol);
			}
		} while (minDeltaCost < -Double.MIN_VALUE);

		return null;
	}

	/**
	 * A main method used for testing the GRASP metaheuristic.
	 * 
	 */
	public static void main(String[] args) throws IOException {

		long startTime = System.currentTimeMillis();
		GRASP_QBFAC grasp = new GRASP_QBFAC(0.1, (long)100000, "instances/qbf100", true);
		Solution<Integer> bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		long endTime   = System.currentTimeMillis();
		long totalTime = endTime - startTime;
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");

	}

}
