SECTIONS
{
	.text 0x7e00 :
	{
		*(.text)
	}
	.data :
	{
		*(.data)
	}
	.rodata :
	{
		*(.rodata)
	}
	.bss :
	{
		*(.bss)
	}
}
