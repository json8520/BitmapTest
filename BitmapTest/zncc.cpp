double CGCPChip::CalculateZNCCCost(double* tgtImg, int tgtCols, int tgtRows, double* refImg, int refCols, int refRows, const vector<int>& nBackCols, const vector<int>& nBackRows, fstream& log)
{

	if (tgtImg == NULL || refImg == NULL)
	{
		log << "One of the input images is empty!" << "\n";
		return -1;
	}//end: if (tgtImg == NULL || refImg == NULL)

	double zncc_cost = 0.0;
	double avg_left = 0., sum_left = 0., sum_sqr_left = 0.;	// parameters for left patch(chip image)
	double avg_right = 0., sum_right = 0., sum_sqr_right = 0.;	// parameters for right patch(target image)
	double sum_cross = 0., zncc_num = 0., zncc_den = 0.;
	double stdev_left, stdev_right;
	double val_left, val_right;
	int c, r;
	int count = 0;
	//** Calculate Statistics
	for (int k = 0; k < (int)nBackCols.size(); ++k)
	{
		// Retirve Non-background pixel location
		c = nBackCols[k];
		r = nBackRows[k];

		val_left = refImg[r * refCols + c];
		val_right = tgtImg[r * tgtCols + c];

		if (val_left <= 0 || val_right <= 0) { continue; }
		else { count++; }

		sum_left += val_left;
		sum_right += val_right;

		sum_sqr_left += val_left * val_left;
		sum_sqr_right += val_right * val_right;

		sum_cross += val_left * val_right;
	}//end: for (int k = 0; k < nBackgrounds.Length(); ++k)	

	 //** Calculate ZNCC(Zero Normailized Cross Correlation)
	avg_left = sum_left / count;
	avg_right = sum_right / count;

	stdev_left = sqrt((sum_sqr_left / count) - (avg_left * avg_left));
	stdev_right = sqrt((sum_sqr_right / count) - (avg_right * avg_right));

	zncc_num = sum_cross - (avg_left * sum_right) - (avg_right * sum_left) + (avg_left * avg_right * count);
	zncc_den = stdev_left * stdev_right * count;

	if (zncc_den == 0)
	{
		cout << "The denominator of the ZNCC cost function is zero!" << "\n";
		log << "The denominator of the ZNCC cost function is zero!" << "\n";
		return -1;
	}//end: if (zncc_den == 0)

	zncc_cost = zncc_num / zncc_den;

	return zncc_cost;
}//** end of member function: CalculateZNCCCost(double* tgtImg, int tgtCols, int tgtRows, double* refImg, int refCols, int refRows, const vector<int>& nBackCols, const vector<int>& nBackRows, fstream& log)
