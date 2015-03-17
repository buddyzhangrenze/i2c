#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	unsigned long pte;
}pte_t;
typedef struct {
	unsigned long pmd[16];
}pmd_t;
typedef struct {
	unsigned long pgd;
}pgd_t;
typedef struct {
	unsigned long pgprot;
}pgport_t;
typedef struct {
	unsigned long pud;
}pud_t;
#define PGDIR_SHIFT 24
#define PMD_SHIFT   21
#define PAGE_SHIFT  12
#define PTRS_PER_PTE 1024
#define PTRS_PER_PMD 1
#define PTRS_PER_PGD 1024
/*
 * pgd
 */

#define pgd_index(address) (((address)>>PGDIR_SHIFT) & (PTRS_PER_PGD - 1))
#define pgd_offset(mm,address) ((mm) + pgd_index((address)))
/*
 * pud
 */
#define pud_offset(pgd,start) (pgd)
/*
 * pmd
 */
static inline unsigned long pmd_index(unsigned long address)
{
	return (address>>PMD_SHIFT) & (PTRS_PER_PMD -1);
}
static inline pmd_t *pmd_offset(pud_t *pud,unsigned long address)
{
	return (pmd_t *)pud + pmd_index(address);
}
/*
 * pte
 */
static inline unsigned long pte_index(unsigned long address)
{
	return (address >> PAGE_SHIFT) & (PTRS_PER_PTE -1);
}
static inline pte_t *pte_offset_kernel(pmd_t *pmd,unsigned long address)
{
	return (pte_t *)(pmd) + pte_index(address);
}

int main()
{
    pte_t *pte_tmp   = NULL;
	pgd_t *pgd_tmp   = NULL;
	pud_t *pud_tmp   = NULL;
	pmd_t *pmd_tmp   = NULL;

	unsigned long pgd = 0x12345678;
	unsigned long va  = 0x87654321;
	/*
	 * pgd
	 */
	pgd_tmp = pgd_offset(pgd,va);
	printf("[%8lx]PGD\n",pgd_tmp);
	/*
	 * pud
	 */
	pud_tmp = pud_offset(pgd_tmp,va);
	printf("[%8lx]PUD\n",pud_tmp);
	/*
	 * pmd
	 */
	pmd_tmp = pmd_offset(pud_tmp,va);
	printf("[%8lx]PMD\n",pmd_tmp);
	/*
	 * pte
	 */
	pte_tmp = pte_offset_kernel(pmd_tmp,va);
	printf("[%8lx]PTE\n",pte_tmp);

	return 0;
}
