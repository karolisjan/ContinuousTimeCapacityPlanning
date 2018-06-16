#if defined(__posix) || defined(__unix) || defined(__linux) || defined(__APPLE__)
    #pragma GCC diagnostic ignored "-Wreorder"
	#pragma GCC diagnostic ignored "-Wsign-compare"
#endif 

#ifndef __BASE_INDIVIDUAL__
#define __BASE_INDIVIDUAL__

#include <vector>
#include <cstdlib>
#include <utility>
#include <algorithm>
#include <functional>

#include "utils.h"


namespace types
{
	/*
		Base dynamic individual class. Not to be called directly.
	*/
	template<class Gene>
	struct BaseIndividual
	{
		explicit BaseIndividual() {}

		template<class... GeneParams>
		explicit BaseIndividual(
			int starting_length = 1,
			double p_xo = 0.820769,
			double p_gene_swap = 0.766782,
			GeneParams... params
		) :
			p_xo(p_xo),
			p_gene_swap(p_gene_swap)

		{
			while (starting_length-- > 0) {
				genes.push_back(std::move(Gene(params...)));
			}
		}

		inline void cross(BaseIndividual &other) 
		{
			if (utils::random() > p_xo) {
				return;
			}

			if (genes.size() < 2 || other.genes.size() < 2) {
				return;
			}

			int i;

			if (genes.size() < other.genes.size()) {
				for (i = 0; i != genes.size(); ++i) {
					if (utils::random() <= 0.50) {
						std::swap(genes[i], other.genes[i]);
					}
				}
				for (; i != other.genes.size(); ++i) {
					if (utils::random() <= 0.50) {
						genes.push_back(other.genes[i]);
					}
				}
			}
			else {
				for (i = 0; i != other.genes.size(); ++i) {
					if (utils::random() <= 0.50) {
						std::swap(genes[i], other.genes[i]);
					}
				}
				for (; i != genes.size(); ++i) {
					if (utils::random() <= 0.50) {
						other.genes.push_back(genes[i]);
					}
				}
			}
		}

		inline void mutate() 
		{
			for (auto &gene : genes) {
				gene.mutate();
			}
			
			add_gene();
			swap_genes();
		}

		std::vector<Gene> genes;

	private:
		inline void add_gene()
		{
			genes.push_back(genes.back().make_new());
		}

		inline void swap_genes()
		{
			if (utils::random() >= p_gene_swap) {
				return;
			}

			int g1, g2;

			do {
				g1 = utils::random_int(0, genes.size());
				g2 = utils::random_int(0, genes.size());
			} while (g1 == g2);

			std::swap(genes[g1], genes[g2]);
		}

		double p_xo, p_gene_swap;
	};
}

#endif 