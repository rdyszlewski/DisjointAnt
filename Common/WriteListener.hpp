class WriteListener
{
public:
	virtual void WriteResult(int iteration, std::vector<int> values) = 0;
	virtual void WriteLog(std::string message) = 0;
};