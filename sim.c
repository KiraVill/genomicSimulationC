#include "sim-operations.h"

/* main, as a simple guide to library functions. */
int main(int argc, char* argv[]) {

	// To show you the speed of the library the function calls are wrapped by
	// this clock call.
	clock_t c;
	c = clock();

	// load our starting data to a SimData struct.
    SimData* sd = create_empty_simdata(time(NULL));
	struct MultiIDSet init = load_data_files(sd, "parents.txt",
            "genetic-map.txt", "qtl.eff.txt", DETECT_FILE_FORMAT);
	// init contains init.group, init.map, and init.effSet

	// prepare to perform a cross by selecting the options we want
	// (macros TRUE and FALSE map to 1 and 0 and are defined in sim-operations.h)
	GenOptions g = {.will_name_offspring=TRUE, .offspring_name_prefix="F1", .family_size=100,
		.will_track_pedigree=TRUE, .will_allocate_ids=TRUE,
		.filename_prefix="testcross", .will_save_pedigree_to_file=FALSE,
		.will_save_bvs_to_file=NO_EFFECTSET, .will_save_alleles_to_file=FALSE,
		.will_save_to_simdata=TRUE};
	// rather than defining all options manually, the option exists to use the constant
	// BASIC_OPT, which has the following definition:
	//	{.will_name_offspring=FALSE, .offspring_name_prefix=NULL, .family_size=1,
	//	.will_track_pedigree=FALSE, .will_allocate_ids=TRUE,
	//	.filename_prefix=NULL, .will_save_pedigree_to_file=FALSE,
	//	.will_save_bvs_to_file=FALSE, .will_save_alleles_to_file=FALSE,
	//	.will_save_to_simdata=TRUE};

	// perform your cross. Multiple crossing functions are available for different
	// purposes - search the docs for functions starting with `cross_` or `make_`
	GroupNum f1 = make_all_unidirectional_crosses(sd, init.group, init.map, g);

	// save data from the results of your cross. Search the docs for functions
	// starting with `save_`
	save_genotypes("f1all.txt", sd, f1, GSC_FALSE);

	// Other operations you might perform at this point:
	// - Further crosses (maybe crossing from generation F1, or another cross from the founders)
	// - Splitting or combining groups to be able to operate on different subsets of a group. Eg
	//		`split_group_by_fitness`, `split_into_families`, `combine_groups`.
	// - Checking which groups currently exist with `get_existing_groups` or
	//		`get_existing_group_counts`
	// - Certain functions are available alongside the basic saving functions that calculate
	// 		additional statistics from genotype data, eg `calculate_optimal_bv`,
	//		`calculate_group_block_effects`, `calculate_recombinations_from_file`
	c = clock() - c;
	printf("The functions took %f seconds to run\n", (double)c / CLOCKS_PER_SEC);

	// Cleanup step: perform this call after you're done with the data in a SimData
	// to free all memory.
	delete_simdata(sd);

	return 0;
}
