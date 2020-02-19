#include <nan.h>
#include <node.h>
#include <v8.h>
#include <time.h>

using v8::Date;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

// No longer needed on Node 13 since setting process.env.TZ will automatically reset the date cache
// https://github.com/bnoordhuis/io.js/commit/eba348b6aecf5c764f786f6f049431237702c88f
// https://github.com/nodejs/node/pull/29504
NAN_METHOD(Reset) {
  Isolate *isolate = Isolate::GetCurrent();
#ifdef _WIN32
  _tzset();
#else
  tzset();
#endif
  auto constexpr time_zone_detection = Isolate::TimeZoneDetection::kRedetect;
  isolate->DateTimeConfigurationChangeNotification(time_zone_detection);
}

NAN_MODULE_INIT(Init) {
  Set(target, New<String>("reset").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Reset)).ToLocalChecked());
}

NODE_MODULE(resetdatecache, Init)
