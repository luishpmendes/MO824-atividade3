package problems.qbfac.solvers;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import metaheuristics.grasp.AbstractGRASP;
import problems.qbfac.QBFAC_Inverse;
import solutions.Solution;

public class ReactiveGRASP_QBFAC extends GRASP_QBFAC {

        protected List<Double> alphas, probabilities, sums;
        protected List<Integer> quantities;
    
	public ReactiveGRASP_QBFAC(Integer iterations, String filename, List<Double> alphas, Boolean bestImproving) throws IOException {
		super(-1.0, iterations, filename, bestImproving);
                this.alphas = alphas;
                createProbabilities();
                //this.verbose = false;
	}
        
        public ReactiveGRASP_QBFAC(Long timeLimit, String filename, List<Double> alphas, Boolean bestImproving) throws IOException {
		super(-1.0, timeLimit, filename, bestImproving);
                this.alphas = alphas;
                createProbabilities();
                //this.verbose = false;
	}
        
        private List<Double> createProbabilities() {
                probabilities = new ArrayList<Double>();
                sums = new ArrayList<Double>();
                quantities = new ArrayList<Integer>();
                for(Double alpha : alphas) {
                        probabilities.add(1.0/alphas.size());
                        sums.add(0.0);
                        quantities.add(0);
                }
                return probabilities;
        }
        
        @Override
        protected Double chooseAlpha() {
            int alphaIndex = alphas.indexOf(alpha);
            if(alphaIndex != -1) {
                sums.set(alphaIndex, sums.get(alphaIndex) + incumbentSol.cost);
                quantities.set(alphaIndex, quantities.get(alphaIndex) + 1);
            }
            
            Boolean canUpdate = true;
            for(Integer qty : quantities) {
                if(qty < 20) {
                    canUpdate = false;
                }
            }
            long endTime   = System.currentTimeMillis();
            long totalTime = endTime - startTime;
            if(totalTime < 0.2 * timeLimit) {
                canUpdate = false;
            }
            if(canUpdate){ 
                updateAlphas();
            }
            
            Double d = rng.nextDouble();
            Double sum = 0.0;
            for(int i = 0; i < probabilities.size(); i++) {
                sum += probabilities.get(i);
                if(d <= sum) {
                    return alphas.get(i);
                }
            }
            return alphas.get(0);
        }
        
        private void updateAlphas() {
            ArrayList<Double> q = new ArrayList<Double>();
            Double sumQ = 0.0;
            for(int i = 0; i < alphas.size(); i++) {
                    Double ai = sums.get(i)/quantities.get(i);
                    Double qi = incumbentSol.cost / ai;
                    //System.out.println("Alpha = " + alphas.get(i) + " Sum = " + sums.get(i) + ", Quantity = " + quantities.get(i) + ", Sol = " + incumbentSol.cost + ", Ai = " + ai + ", qi = " + qi);
                    q.add(qi);
                    sumQ += qi;
            }
            for(int i = 0; i < alphas.size(); i++) {
                Double pi = q.get(i)/sumQ;
                probabilities.set(i, pi);
                //System.out.println(pi);
            }
            //System.out.println("------------------------------------------");
        }

        public static void main(String[] args) throws IOException {
                
                long testTime = 180000;
                //long testTime = 1000;
                
                // Instance: 20
                // Alphas: [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
                // Best Improving
		System.out.println("Instance = 20, Alphas = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], Best Improving");
                long startTime = System.currentTimeMillis();
                List<Double> alphas = Arrays.asList(0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
		ReactiveGRASP_QBFAC grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf020", alphas, true);
		Solution<Integer> bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		long endTime   = System.currentTimeMillis();
		long totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 20
                // Alpha: [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0]
                // Best Improving
		System.out.println("Instance = 20, Alphas = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0], Best Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf020", alphas, true);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");

                // Instance: 40
                // Alphas: [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
                // Best Improving
		System.out.println("Instance = 40, Alphas = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], Best Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf040", alphas, true);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");

                // Instance: 40
                // Alpha: [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0]
                // Best Improving
		System.out.println("Instance = 40, Alphas = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0], Best Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf040", alphas, true);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 60
                // Alphas: [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
                // Best Improving
		System.out.println("Instance = 60, Alphas = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], Best Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf060", alphas, true);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 60
                // Alpha: [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0]
                // Best Improving
		System.out.println("Instance = 60, Alphas = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0], Best Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf060", alphas, true);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 80
                // Alphas: [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
                // Best Improving
		System.out.println("Instance = 80, Alphas = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], Best Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf080", alphas, true);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 80
                // Alpha: [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0]
                // Best Improving
		System.out.println("Instance = 80, Alphas = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0], Best Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf080", alphas, true);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 100
                // Alphas: [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
                // Best Improving
		System.out.println("Instance = 100, Alphas = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], Best Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf100", alphas, true);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 100
                // Alpha: [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0]
                // Best Improving
		System.out.println("Instance = 100, Alphas = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0], Best Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf100", alphas, true);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 20
                // Alphas: [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
                // First Improving
		System.out.println("Instance = 20, Alphas = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], First Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf020", alphas, false);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 20
                // Alpha: [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0]
                // First Improving
		System.out.println("Instance = 20, Alphas = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0], First Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf020", alphas, false);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 40
                // Alphas: [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
                // First Improving
		System.out.println("Instance = 40, Alphas = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], First Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf040", alphas, false);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 40
                // Alpha: [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0]
                // First Improving
		System.out.println("Instance = 40, Alphas = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0], First Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf040", alphas, false);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 60
                // Alphas: [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
                // First Improving
		System.out.println("Instance = 60, Alphas = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], First Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf060", alphas, false);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 60
                // Alpha: [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0]
                // First Improving
		System.out.println("Instance = 60, Alphas = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0], First Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf060", alphas, false);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 80
                // Alphas: [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
                // First Improving
		System.out.println("Instance = 80, Alphas = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], First Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf080", alphas, false);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 80
                // Alpha: [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0]
                // First Improving
		System.out.println("Instance = 80, Alphas = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0], First Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf080", alphas, false);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 100
                // Alphas: [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
                // First Improving
		System.out.println("Instance = 100, Alphas = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], First Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf100", alphas, false);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");
                
                System.out.println("-------------------------------------------------------------------------------------------------------------------");
                
                // Instance: 100
                // Alpha: [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0]
                // First Improving
		System.out.println("Instance = 100, Alphas = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0], First Improving");
                startTime = System.currentTimeMillis();
                alphas = Arrays.asList(0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0);
		grasp = new ReactiveGRASP_QBFAC(testTime, "instances/qbf100", alphas, false);
		bestSol = grasp.solveLimitedByTime();
		System.out.println("maxVal = " + bestSol);
		endTime   = System.currentTimeMillis();
		totalTime = endTime - startTime;
                System.out.println("Last: " + grasp.toString());
		System.out.println("Time = "+(double)totalTime/(double)1000+" seg");

	}
        
        @Override
        public String toString() {
            String result = "[";
            for(int i = 0; i < alphas.size(); i++) {
                    result+="p("+alphas.get(i)+") = "+probabilities.get(i)+", ";
            }
            result = result.substring(0, result.length() - 2);
            result+="]";
            return "(Iter. " + currentIteration + ") BestSol = " + bestSol + ", IncumbentSol = " + incumbentSol + ", Current Alpha: " + alpha + ", Alphas = " + result;
        }

}

