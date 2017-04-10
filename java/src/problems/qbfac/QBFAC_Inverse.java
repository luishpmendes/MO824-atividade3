package problems.qbfac;

import java.io.IOException;

/**
 * Class representing the inverse of the Quadractic Binary Function
 * ({@link QBF}), which is used since the GRASP is set by
 * default as a minimization procedure.
 * 
 * @author ccavellucci, fusberti
 */
public class QBFAC_Inverse extends QBFAC {

	/**
	 * Constructor for the QBF_Inverse class.
	 * 
	 * @param filename
	 *            Name of the file for which the objective function parameters
	 *            should be read.
	 * @throws IOException
	 *             Necessary for I/O operations.
	 */
	public QBFAC_Inverse(String filename) throws IOException {
		super(filename);
	}


	/* (non-Javadoc)
	 * @see problems.qbf.QBF#evaluate()
	 */
	@Override
	public Double evaluateQBFAC() {
		return -super.evaluateQBFAC();
	}
	
	/* (non-Javadoc)
	 * @see problems.qbf.QBF#evaluateInsertion(int)
	 */
	@Override
	public Double evaluateInsertionQBFAC(int i) {	
		return -super.evaluateInsertionQBFAC(i);
	}
	
	/* (non-Javadoc)
	 * @see problems.qbf.QBF#evaluateRemoval(int)
	 */
	@Override
	public Double evaluateRemovalQBFAC(int i) {
		return -super.evaluateRemovalQBFAC(i);
	}
	
	/* (non-Javadoc)
	 * @see problems.qbf.QBF#evaluateExchange(int, int)
	 */
	@Override
	public Double evaluateExchangeQBFAC(int in, int out) {
		return -super.evaluateExchangeQBFAC(in,out);
	}

}
