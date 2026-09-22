int	ft_isprint(int c)
{
	c = (unsigned char)c;
	return (c >= ' ' && c <= '~');
}
