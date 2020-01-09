#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <advanced_cxx/bmp.h>
#include <advanced_cxx/bmp_grapher.pb.h>

namespace mabz { namespace graphers {

class BmpGrapher
{
protected:
	mabz::Bmp mBmp;
	const mabz::PixelXYMapper mPixelXYMapper;

public:
	BmpGrapher(
		double xCenter, 
		double yCenter, 
		double xDomainWidth, 
		int pixelWidth,
		int pixelHeight)

		: mBmp(pixelWidth, pixelHeight)
		, mPixelXYMapper(xCenter, yCenter, xDomainWidth, pixelWidth, pixelHeight)
	{}
	BmpGrapher(const BmpGrapher&) = delete;
	virtual ~BmpGrapher() {}

	class RunArgs
	{
	public:
		virtual ~RunArgs() {}
	};
	virtual void Run(std::shared_ptr<const BmpGrapher::RunArgs>) = 0;

	bool WriteToFile(const char* filename) const;
};

class BmpGrapherFactory
{
private:
	struct RunPair
	{
		std::shared_ptr<BmpGrapher> mGrapher;
		std::shared_ptr<const BmpGrapher::RunArgs> mRunArgs;
		RunPair(RunPair&&) = default;
	};

	// Pre-cached runs to generate bmps (just add water by calling the combos of args).
	std::vector<RunPair> mPendingGraphs;
	
	// must use a static method to create. Constructor is private.
	BmpGrapherFactory() {}

public:
	static BmpGrapherFactory* NewFromPbufJsonFile(const char* filename, std::string& outErrorStr);

	// I am really not doing much error checking at the moment lol.
	void Run();
};

} /* namespace graphers */
} /* namespace mabz */
