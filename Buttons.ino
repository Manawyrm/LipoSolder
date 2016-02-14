void statusPageButton()
{

}

void presetPageButton()
{

}

void changePageButton()
{
	currentPage += 1;
	if (currentPage >= sizeof(pages) / sizeof(void*))
		currentPage = 0;
}